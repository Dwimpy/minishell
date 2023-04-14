/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/05 22:15:18 by dwimpy            #+#    #+#             */
/*   Updated: 2023/04/14 14:21:30 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "tokenizer.h"
#include "environment_handler.h"

#include "x_execution.h"
#include "get_next_line.h"
#include <curses.h>
#include <term.h>
int		ft_execute(t_input *input, t_ast_node *root, int *fd);
int	ft_execute_tree(t_input *input, t_ast_node *root, int *fd, int subshell);

int		ft_pipe(t_input *input, t_ast_node *root, int *fd);
int		ft_and_if(t_input *input, t_ast_node *root, int *fd, int *status);
int		ft_or_if(t_input *input, t_ast_node *root, int *fd, int *status);


int	main(int argc, char **argv, char **envp)
{
	t_input				input;
	int					fd;
	int					exit_code;


	//ft_signals(&sa);
	//input.root->is_subshell_start = 0;

	init_input(&input, envp);
	while (1)
	{
		if (gen_input(&input) == 1)
			continue ;
		parse_all_input(&input);

		// my part
		fd = 0;
		exit_code = ft_execute(&input, input.root, &fd);

		ast_del_node(input.root);
		input.root = NULL;
		free(input.lexer.input);
		free_token_list(input.tokens);
	}
	return (0);
}

int	ft_execute(t_input *input, t_ast_node *root, int *fd)
{
	int			exit_code;
	pid_t		pid;
	int			status;

	
	
	if (root->type == COMMAND) // only one thing to execute
	{
		if (root->is_subshell != 1)
			exit_code = ft_command(root->data.command.cmd.args, input, root);
		else
		{
		//	exit_code = ft_subshell();
			return (exit_code);
		}
		return (exit_code);
	}
	while (root->left != NULL)
		root = root->left;
	exit_code = ft_execute_tree(input, root, fd, 0);

	pid = 1;
	while (pid != -1)
		pid = waitpid(-1, &status, 0);
	while (*fd > 2)
		close((*fd)--);
	return (exit_code);
}

// int	ft_subshell(t_input *input, t_ast_node *root, int *fd, int *pid)
// {
// 	int				new_fd[2];
// 	int 			pid;
// 	int 			status;

// 	status = 0;
// 	if (pipe(new_fd) == -1)
// 		return (-1);
// 	pid = fork();
// 	if (pid == -1)
// 		return (-1);
// 	if (pid == 0)
// 	{
// 		close(new_fd[0]);
// 		dup2(new_fd[1], STDOUT_FILENO);
// 		close(new_fd[1]);
// 		status = ft_execute_tree(input, root, fd, 1);
// 		exit(status);
// 	}
// 	else
// 	{
// 		close(new_fd[1]);
// 		*fd = new_fd[0];
// 		if ((!root->parent->parent && root->parent->left != root) || !((root->parent->parent && (root->parent->parent->type == PIPELINE)) || (root->parent->left == root && root->parent->type == PIPELINE)))
// 			waitpid(pid, &status, 0);
// 		*pid = pid;
// 		return (status);
// 	}
// }

// int	ft_subshell_end(t_input *input, t_ast_node *root, int *fd)
// {
// 	exit
// }

int	ft_execute_tree(t_input *input, t_ast_node *root, int *fd, int subshell)
{
	int				exit_code;
	int				status;
	int				stdin_cp;
	t_node_type		type;
	int 			pid;


	status = 1;
	pid = 0;
	exit_code = 0;
	while (root)
	{
		printf("subshell :%d\n", root->is_subshell);
		if (root->is_subshell == 1)
			printf("subshell\n");
		// {
		// 	if (subshell == 0)
		// 	{
		// 		//subshell = 1;
		// 		exit_code = ft_subshell(input, root, fd, &pid);
		// 	}
		// }
		// else if (subshell == 1)
		// {
		// 	if (pid == 0)
		// 		exit(exit_code);
		// 	//subshell = 0;
		// }

		if (root->parent->left == root)
			type = root->parent->type;
		else if (root->parent->parent)
			type = root->parent->parent->type;
		else
		{
			if (status == 0)
				return (exit_code);
			if (root->parent->type == PIPELINE)
			{
				stdin_cp = dup(STDIN_FILENO);
				close (STDIN_FILENO);
				dup2(*fd, STDIN_FILENO);
				exit_code = ft_command(root->data.command.cmd.args, input, root);
				dup2(stdin_cp, STDIN_FILENO);
				close(stdin_cp);
				return (exit_code);
			}
			return (ft_command(root->data.command.cmd.args, input, root));
		}


		if (type == PIPELINE && status)
			exit_code = ft_pipe(input, root, fd);
		else if (type == AND_IF)
			exit_code = ft_and_if(input, root, fd, &status);
		else if (type == OR_IF)
			exit_code = ft_or_if(input, root, fd, &status);

		if (root->parent->left == root)
			root = root->parent->right;
		else
			root = root->parent->parent->right;
		
	}
	return (exit_code);
}

int	ft_pipe(t_input *input, t_ast_node *root, int *fd)
{
	static int		new_fd[2];
	int 			pid;
	int 			status;

	status = 0;
	if (pipe(new_fd) == -1)
		return (-1);
	pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0)
	{
		close(new_fd[0]);
		if ((root->parent->parent && (root->parent->parent->type == PIPELINE)) || (root->parent->left == root && root->parent->type == PIPELINE))
				dup2(new_fd[1], STDOUT_FILENO);
		dup2(*fd, STDIN_FILENO);
		status = ft_command(root->data.command.cmd.args, input, root);
		close(new_fd[1]);
		close(*fd);
		exit(status);
	}
	else
	{
		close(new_fd[1]);
		*fd = new_fd[0];
		if ((!root->parent->parent && root->parent->left != root) || !((root->parent->parent && (root->parent->parent->type == PIPELINE)) || (root->parent->left == root && root->parent->type == PIPELINE)))
			waitpid(pid, &status, 0);
		return (status);
	}
}

int	ft_and_if(t_input *input, t_ast_node *root, int *fd, int *st)
{
	int 			pid;
	int 			status;
	int 			last_status;
	int				stdin_cp;

	if (root == root->parent->left || !(root->parent->type == PIPELINE))
		last_status = ft_command(root->data.command.cmd.args, input, root);
	else
	{
		stdin_cp = dup(STDIN_FILENO);
		close (STDIN_FILENO);
		dup2(*fd, STDIN_FILENO);
		last_status = ft_command(root->data.command.cmd.args, input, root);
		dup2(stdin_cp, STDIN_FILENO);
		close(stdin_cp);
	}
	if (last_status == 0)
		*st = 1;
	else
		*st = 0;

	while (pid != -1)
		pid = waitpid(-1, &status, 0);
	while (*fd > 2)
		close((*fd)--);
	return (last_status);
}

int	ft_or_if(t_input *input, t_ast_node *root, int *fd, int *st)
{
	int 			pid;
	int 			status;
	int 			last_status;
	int				stdin_cp;

	if (root == root->parent->left || !(root->parent->type == PIPELINE))
		last_status = ft_command(root->data.command.cmd.args, input, root);
	else
	{
		stdin_cp = dup(STDIN_FILENO);
		close (STDIN_FILENO);
		dup2(*fd, STDIN_FILENO);
		last_status = ft_command(root->data.command.cmd.args, input, root);
		dup2(stdin_cp, STDIN_FILENO);
		close(stdin_cp);
	}
	if (last_status != 0)
		*st = 1;
	else
		*st = 0;

	while (pid != -1)
		pid = waitpid(-1, &status, 0);
	while (*fd > 2)
		close((*fd)--);
	return (last_status);
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


// int	ft_execute(t_input *input, t_ast_node *root, int *fd, int *ptr)
// {
// 	char			**arr;
// 	int 			status;
// 	int				permission;
// 	int				subshell;

// 	permission = 0;
// 	if (ptr == NULL)
// 		ptr = &permission;
// 	if (root->is_subshell == 1)
// 	{
// 		//fork()
// 		if (pid = 0)
// 			ft_subshell();
// 		else
// 		if (root->is_subshell_start == 0)
// 		{
// 			printf("subshell\n");
// 			root->is_subshell_start = 1;
// 		}
// 		else
// 		{
// 			return (status);
// 		}
// 		// if subshell
		
// 		// status = ft_subshell(input, root, fd, 1);
		
// 		// is not done
// 		return (status);
// 	}
// 	if (root->type == COMMAND && root == input->root) // only one thing to execute
// 		return (ft_command(root->data.command.cmd.args, input));



// 	if (root->left)
// 	{
// 		status = ft_execute(input, root->left, fd, ptr); // executes the left
// 		if ((root->type == OR_IF && status == 0) || (root->type == AND_IF && status != 0) || *ptr == 1) // checks whether the next one needs to be executed or not
// 		{
// 			while (pid != -1)
// 				pid = waitpid(-1, &status, 0);
// 			if (*ptr != 1)
// 			{
// 				*ptr = 1;
// 				return (status);
// 			}
// 			else if (root->type == PIPELINE)
// 					return (status);
// 		}
// 		return (ft_execute(input, root->right, fd, ptr)); // executes the rigth
// 	}
// 	else if (root->parent && ((root->parent->left == root && (root->parent->type == AND_IF || root->parent->type == OR_IF)))) // executes the left if it is a || or a &&
// 	{
// 		return (ft_command(root->data.command.cmd.args, input));
// 	}
// 	else if (root->parent && (root->parent->parent && (root->parent->left != root && (root->parent->parent->type == AND_IF || root->parent->parent->type == OR_IF)))) // executes the right if it is a || or a &&
// 	{
		
// 		if (root->parent->type == AND_IF || root->parent->type == OR_IF)
// 			return (ft_command(root->data.command.cmd.args, input)); // last one wasn't a pipe. No input inputfile
// 		else
// 			return (ft_pipe(input, root, fd, 1)); // last one was a pipe. It hase a inputfile
// 	}
// 	else
// 		return (ft_pipe(input, root, fd, 0)); // its a pipe
// }





// int	ft_subshell(t_input *input, t_ast_node *root, int *fd, int last)
// {
// 	int				new_fd[2];
// 	int 			pid;
// 	int 			status;
// 	char			*str;

// 	if (pipe(new_fd) == -1)
// 		return (1);
// 	pid = fork();
// 	if (pid == -1)
// 		return (-1);
// 	if (pid == 0)
// 	{
// 		close(new_fd[0]);
// 		if (!last)
// 			if (input->root->right != root)
// 				dup2(new_fd[1], STDOUT_FILENO);
// 		dup2(*fd, STDIN_FILENO);
		
// 		str = (char *)hashmap_get(input->hashmap, "_");
// 		// if (!str)
// 		// 	str = ttyname();
		
// 		printf("%s\n", str);
// 		status = ft_command(&str, input);
// 		//tputs(str, int affcnt, int (*putc)(int));
// 		printf("lol\n");
// 		free(str);
		
// 		close(new_fd[1]);
// 		exit(status);
// 	}
// 	else
// 	{
// 		close(new_fd[1]);
// 		waitpid(pid, &status, 0);
// 		if (*fd != -1)
// 			close(*fd);
// 		*fd = new_fd[0];
// 		if (input->root->right == root)
// 		{
// 			close(*fd);	
// 			*fd = -1;
// 		}
// 		return (status);
// 	}
// }
