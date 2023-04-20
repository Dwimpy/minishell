/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   x_pipe_and_or_if.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 15:15:58 by tkilling          #+#    #+#             */
/*   Updated: 2023/04/20 02:21:03 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "x_execution.h"

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
		//if (!root->is_subshell)
		if (root->parent && root->parent->type == PIPELINE && !(root->is_subshell > root->parent->is_subshell))
			dup2(new_fd[1], STDOUT_FILENO);
		dup2(*fd, STDIN_FILENO);
		status = ft_command(root->data.command.cmd.args, input, root, pid);
		close(new_fd[1]);
		close(*fd);
		exit(status);
	}
	else
	{
		close(new_fd[1]);
		*fd = new_fd[0];
		if (!root->parent || (root->parent && (root->parent->type != PIPELINE)))
			waitpid(pid, &status, 0);
		return (status);
	}
}

int	ft_and_if(t_input *input, t_ast_node *root, int *fd)
{
	int 			pid;
	int 			st;
	int				status;

	st = 0;
	while (pid != -1)
		pid = waitpid(-1, &st, 0);
	while (*fd > 2)
		close((*fd)--);

	if (root->parent && root->parent->type == PIPELINE)
		status = ft_pipe(input, root, fd);
	else
		status = ft_command(root->data.command.cmd.args, input, root, pid);
	return (status);
}

int	ft_or_if(t_input *input, t_ast_node *root, int *fd)
{
	int 			pid;
	int 			st;
	int				status;

	st = 0;
	while (pid != -1)
		pid = waitpid(-1, &st, 0);
	while (*fd > 2)
		close((*fd)--);

	if (root->parent && root->parent->type == PIPELINE)
		status = ft_pipe(input, root, fd);
	else
		status = ft_command(root->data.command.cmd.args, input, root, pid);
	return (status);
}
