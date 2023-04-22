/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   x_execute.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 15:11:41 by tkilling          #+#    #+#             */
/*   Updated: 2023/04/22 21:29:41 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "x_execution.h"

void	ft_change_tree(t_ast_node *root)
{
	t_ast_node *ptr;

	ptr = root->parent;
	while (ptr)
	{
		ptr->right->left = ptr->right->parent;
		ptr->right->parent = ptr->parent;
		ptr = ptr->parent;
	}
}

void	ft_change_tree_back(t_ast_node *root)
{
	t_ast_node *ptr;

	ptr = root->parent;
	while (ptr)
	{
		ptr->right->left = NULL;
		ptr = ptr->parent;
	}
}


int	ft_execution(t_input *input, t_ast_node *root, int *fd)
{
	int			exit_code;
	pid_t		pid;
	int			status;

	exit_code = 0;
	while (root->left != NULL)
		root = root->left;
	ft_change_tree(root);
	exit_code = ft_execute_tree(input, root, fd, 0);
	ft_change_tree_back(root);
	pid = 1;
	while (pid != -1)
		pid = waitpid(-1, &status, 0);
	while (*fd > 2)
		close((*fd)--);
	return (exit_code);
}

int	ft_add_subshell(t_input *input, t_ast_node *root, int *fd, int subshell);

int	ft_execute_tree(t_input *input, t_ast_node *root, int *fd, int subshell)
{
	int				exit_code;

	exit_code = 0;
	while (root)
	{
		if (root->type == COMMAND)
		{
			if (root->parent == NULL && root->left == NULL)
			{
				if (root->is_subshell == 0)
					return (ft_command(root->data.command.cmd.args, input, root));
				else
					return (ft_subshell_no_tree(input, root));
			}
			else if (root->is_subshell != subshell)
			{
				exit_code = ft_add_subshell(input, root, fd, subshell + 1);
				while (root->parent && root->parent->is_subshell > subshell)
					root = root->parent;
			}
			else
			{
				if (root->is_subshell == subshell && root->left == NULL)
				{
				//	printf("%d\n", subshell);
					// printf("%d\n", subshell);
					if (root->parent->type == PIPELINE)
						exit_code = ft_pipe(input, root, fd);
					else
						exit_code = ft_and_if(input, root, fd);
				}
				else if (root->is_subshell == subshell)
				{
					// printf("%d\n", subshell);
					// printf("lol\n");
					if (root->left->type == PIPELINE)
						exit_code = ft_pipe(input, root, fd);
					else if (root->left->type == AND_IF && exit_code == 0)
						exit_code = ft_and_if(input, root, fd);
					else if (root->left->type == OR_IF && exit_code != 0)
						exit_code = ft_or_if(input, root, fd);
					else
					{
						while (root->parent && root->parent->type == PIPELINE)
							root = root->parent;
					}
				}
			}
			
		}
		else
		{
			if (root->is_subshell < subshell)
				exit(exit_code);
			if (root->right->is_subshell != subshell)
			{
				if ((exit_code == 0 && root->type == AND_IF) || (exit_code != 0 && root->type == OR_IF) || root->type == PIPELINE)
					exit_code = ft_add_subshell(input, root->right, fd, subshell + 1);
				while (root->parent && root->parent->is_subshell > subshell)
					root = root->parent;
			}
			else
			{
				if (root->right->is_subshell == subshell)
				{
				//	printf("%d\n", subshell);
					if (root->type == PIPELINE)
						exit_code = ft_pipe(input, root->right, fd);
					else if (root->type == AND_IF && exit_code == 0)
						exit_code = ft_and_if(input, root->right, fd);
					else if (root->type == OR_IF && exit_code != 0)
						exit_code = ft_or_if(input, root->right, fd);
					else
					{
						while (root->parent && root->parent->type == PIPELINE)
							root = root->parent;
					}
				}
				// else
				// {
				// 	exit_code = ft_add_subshell(input, root, fd, subshell + 1);
				// 	// while (root->parent && root->parent->is_subshell > subshell)
				// 	// 	root = root->parent;
				// }


			}

		}
		root = root->parent;
	}
	if (subshell)
		exit(exit_code);
	return (exit_code);
}


int	ft_add_subshell(t_input *input, t_ast_node *root, int *fd, int subshell)
{
	static int		new_fd[2];
	int 			pid;
	int 			status;
	t_ast_node		*ptr;
	
	status = 0;
	ptr = root;
	while (ptr && ptr->is_subshell >= subshell)
		ptr = ptr->parent;
	// if (root->is_subshell > subshell)
	// {
	// 	if (ptr->is_subshell <= subshell - 2)
	// 	{
	// 		return (1);
	// 	}
	// }

	if (pipe(new_fd) == -1)
		return (-1);
	pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0)
	{
		ft_signals_child(&(input->sa));
		close(new_fd[0]);
		//if (root->parent && root->parent->type == PIPELINE)
		
		
		if (ptr && ptr->type == PIPELINE)
		{
			// printf("pipe\n");
			dup2(new_fd[1], STDOUT_FILENO);
			// printf("pipe\n");
		}
		status = ft_execute_tree(input, root, fd, subshell);
		close(new_fd[1]);
		close(*fd);
		exit(status);
	}
	else
	{
		close(new_fd[1]);
		*fd = new_fd[0];
		if (ptr == NULL || !(ptr->type == PIPELINE))
			waitpid(pid, &status, 0);
		//printf("status: %d\n", WEXITSTATUS(status));
		return (WEXITSTATUS(status));
	}
}
