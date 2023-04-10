/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkilling <tkilling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/05 22:15:18 by dwimpy            #+#    #+#             */
/*   Updated: 2023/04/10 12:28:52 by tkilling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "environment_handler.h"

#include "x_execution.h"
#include "get_next_line.h"
int	ft_execute(t_input *input, t_ast_node *root, int *fd, int *ptr);
int	ft_pipe(t_input *input, t_ast_node *root, int *fd, int last);

int	main(int argc, char **argv, char **envp)
{
	t_input				input;
	int					fd;
	char				*str;
	int					exit_code;
	struct sigaction	sa;


	//ft_signals(&sa);
	init_input(&input, envp);
	while (1)
	{
		if (generate_input(&input) != 0)
			continue ;
		parse_all_input(&input);

		// my part
		fd = -1;
		exit_code = ft_execute(&input, input.root, &fd, NULL);
		if (fd != -1)
		{
			close(fd);
		}
		//print_tree(input.root);
		// printf("PRINT: %s\n", input->root->left->data.and_if.symbol);
		ast_del_node(input.root);
		input.root = NULL;
		free(input.lexer.input);
		free_token_list(input.tokens);
	}
	return (0);
}

// int	ft_signals(struct sigaction *sa)
// {
// 	sa->sa_flags = SA_SIGINFO;
// 	sa->sa_sigaction = ft_signal_handler;
// 	if (sigaction(SIGINT, &sa, NULL) != 0
// 		|| sigaction(SIGQUIT, &sa, NULL) != 0)
// 	{
// 		write(2, "sigaction error\n", 16);
// 		return (1);
// 	}
// }

// void	ft_signal_handler(int sig, siginfo_t *info, void *context)
// {
// 	(void)context ;
	
// }


int	ft_execute(t_input *input, t_ast_node *root, int *fd, int *ptr)
{
	char			**arr;
	int 			status;
	int				permission;

	permission = 0;
	if (ptr == NULL)
		ptr = &permission;
	if (root->type == COMMAND && root == input->root) // only one thing to execute
		return (ft_command(root->data.command.cmd.args, input));
	if (root->left)
	{
		status = ft_execute(input, root->left, fd, ptr);
		if ((root->type == OR_IF && status == 0) || (root->type == AND_IF && status != 0))
		{
			*ptr = 1;
			return(status);
		}
		if (*ptr == 1)
			if (root->type == PIPELINE)
				return (status);
		return (ft_execute(input, root->right, fd, ptr));
	}
	else if (root->is_subshell == 1)
	{
		// if subshell

		// is not done
		return (status);
	}
	else if (root->parent && ((root->parent->left == root && (root->parent->type == AND_IF || root->parent->type == OR_IF))))
	{
		return (ft_command(root->data.command.cmd.args, input));
	}
	else if (root->parent && (root->parent->parent && (root->parent->left != root && (root->parent->parent->type == AND_IF || root->parent->parent->type == OR_IF))))
	{
		if (root->parent->type == AND_IF || root->parent->type == OR_IF)
			return (ft_command(root->data.command.cmd.args, input));
		else
			return (ft_pipe(input, root, fd, 1));
	}
	else
		return (ft_pipe(input, root, fd, 0));
}

int	ft_pipe(t_input *input, t_ast_node *root, int *fd, int last)
{
	int				new_fd[2];
	int 			pid;
	int 			status;

	if (pipe(new_fd) == -1)
		return (1);
	pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0)
	{
		close(new_fd[0]);
		if (!last)
			if (input->root->right != root)
				dup2(new_fd[1], STDOUT_FILENO);
		dup2(*fd, STDIN_FILENO);
		status = ft_command(root->data.command.cmd.args, input);
		close(new_fd[1]);
		exit(status);
	}
	else
	{
		close(new_fd[1]);
		waitpid(pid, &status, 0);
		if (*fd != -1)
			close(*fd);
		*fd = new_fd[0];
		if (input->root->right == root)
		{
			close(*fd);	
			*fd = -1;
		}
		return (status);
	}
}