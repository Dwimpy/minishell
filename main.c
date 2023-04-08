/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkilling <tkilling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/05 22:15:18 by dwimpy            #+#    #+#             */
/*   Updated: 2023/04/08 14:25:18 by tkilling         ###   ########.fr       */
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
int	ft_execute(t_input *input, t_ast_node *root, int fd);

int	main(int argc, char **argv, char **envp)
{
	t_input			input;
	int				fd;
	char			*str;

	init_input(&input, envp);
	while (1)
	{
		if (generate_input(&input) != 0)
			continue ;
		parse_all_input(&input);

		// my part
		fd = ft_execute(&input, input.root, -1);
		if (fd)
		{
			str = get_next_line(fd);
			while (str)
			{
				printf("%s", str);
				free(str);
				str = get_next_line(fd);
			}
			//close(fd);
		}
		//write(1, "lol", 4);
		//int	i = 0;
		// while (envp[i] != NULL)
		// 	printf("%s\n", envp[i++]);
		// my part end

		//print_tree(input.root);
		// printf("PRINT: %s\n", input->root->left->data.and_if.symbol);
		ast_del_node(input.root);
		input.root = NULL;
		free(input.lexer.input);
		free_token_list(input.tokens);
	}
	return (0);
}

int	ft_execute(t_input *input, t_ast_node *root, int fd)
{
	char			**arr;
	int 			pid;
	int				new_fd[2];
		
	char *str;
	// if (fd != -1)
	// {
	// 	str = get_next_line(fd);
	// 	while (str)
	// 	{
	// 		printf("%s", str);
	// 		free(str);
	// 		str = get_next_line(fd);
	// 		write(1, "\nwohoo\n\n", 9);
	// 	}
	// 	//close(fd);
	// }

	if (root == NULL)
		return (0);
	if (!(root->right) && !(root->left) && !(root->parent) && fd == -1) // only one thing to execute
	{
		// if (root->type == COMMAND && root->data.command.cmd.name_path == NULL)
		// 	if (root->data.command.cmd.assignments)
		// 		return(0);
		ft_command(root->data.command.cmd.args, input);
		return (0);
	}
	
	//ft_execute(input, root->left);
	if (root->left)
	{
		fd = ft_execute(input, root->left, fd);
		
		// if (fd != -1)
		// {
		// 	str = get_next_line(fd);
		// 	while (str)
		// 	{
		// 		printf("%s", str);
		// 		free(str);
		// 		str = get_next_line(fd);
		// 	}
		// 	write(1, "\nwahoo\n\n", 9);
		// 	//close(fd);
		// }
		// if (root->type == AND_IF)
		// 	printf("and if %s\n", (root->data.and_if.symbol));
		// if (root->type == COMMAND)
		// 	printf("command %s\n", (root->data.command.cmd.name_path));
		// if (root->type == OR_IF)
		// 	printf("or if %s\n", (root->data.or_if.symbol));
		// if (root->type == PIPELINE)
		// 	printf("pipe %c\n", (root->data.pipeline.symbol));
		fd = ft_execute(input, root->right, fd);
		return (fd);
		// if (root->type == SUBSHELL)
		// 	printf("subshell %s\n", (root->data.subshell.root.));
		// else if (!(ft_memcmp("||", *(root->data.command.cmd.args), 3)))
		// 	printf("pipe pipe\n");
		// else if (!(ft_memcmp("&&", *(root->data.command.cmd.args), 3)))
		// 	printf("and and\n");
		// else
		// 	printf("no thing");

		

		
		//ft_command(root->data.command.cmd.args, input);



		
		// if (root->left->parent->type == PIPELINE)
		// 	printf("REDIRECT INPUT to child 2\n");
		// print_node(root->left);
		// printf("left\n");
	}
	else
	{
		// int 			pid;
		// int				new_fd[2];
		
		if (root->type != COMMAND)
			exit (0);
		if (root->type == COMMAND && root->data.command.cmd.name_path == NULL)
			if (root->data.command.cmd.assignments)
				exit(1);
		if (pipe(new_fd) == -1)
			return (1);
		pid = fork();
		if (pid == -1)
			return (-1);
		if (pid == 0)
		{
			close(new_fd[0]);
			dup2(new_fd[1], STDOUT_FILENO);
			dup2(fd, STDIN_FILENO);
			ft_command(root->data.command.cmd.args, input);
			close(new_fd[1]);
			exit(0);
		}
		else
		{
			close(new_fd[1]);
			//read(fd[0], name, 1000);
			//close(fd[0]);
			waitpid(pid, NULL, 0);
			if (fd != -1)
				close(fd);
			// if (new_fd[0] != -1)
			// {
			// 	str = get_next_line(new_fd[0]);
			// 	while (str)
			// 	{
			// 		printf("%s", str);
			// 		free(str);
			// 		str = get_next_line(new_fd[0]);
			// 	}
			// 	write(1, "\nwahoo\n\n", 9);
			// 	//close(fd);
			// }
			if (fd != -1)
				close(fd);
			return (new_fd[0]);
		}
	}
	// if (root->right)
	// {
	// 	// print_node(root->right);
	// 	// printf("right\n");
	// }
	
	// if ((root->right) && (root->left))
	// {
	// 	print_node(root);
	// 	printf("parent\n");
	// }
	
	// else
	// 	printf("\nno\n");
	//ft_execute(input, root->right);
}
