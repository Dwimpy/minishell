/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   x_pipe_and_or_if.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkilling <tkilling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 15:15:58 by tkilling          #+#    #+#             */
/*   Updated: 2023/04/24 11:26:54 by tkilling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "x_execution.h"

int	ft_pipe_helper(int pid, t_input *input, int new_fd[2], t_ast_node *root);

int	ft_pipe(t_input *input, t_ast_node *root, int *fd)
{
	static int		new_fd[2];
	int				pid;
	int				status;

	status = 0;
	if (pipe(new_fd) == -1)
		return (-1);
	pid = fork();
	if (ft_pipe_helper(pid, input, new_fd, root) == -1)
		return (-1);
	if (pid == 0)
	{
		dup2(*fd, STDIN_FILENO);
		status = ft_command(root->data.command.cmd.args, input, root);
		return (ft_close_and_exit(new_fd, fd, status));
	}
	else
	{
		*fd = new_fd[0];
		if (!root->parent || (root->parent && (root->parent->type != PIPELINE)))
			waitpid(pid, &status, 0);
		return (WEXITSTATUS(status));
	}
}

int	ft_pipe_helper(int pid, t_input *input, int new_fd[2], t_ast_node *root)
{
	if (pid == -1)
		return (-1);
	if (pid == 0)
	{
		ft_signals_child(&(input->sa));
		close(new_fd[0]);
		if (root->parent && root->parent->type == PIPELINE
			&& !(root->is_subshell > root->parent->is_subshell))
			dup2(new_fd[1], STDOUT_FILENO);
	}
	else
	{
		close(new_fd[1]);
	}
	return (0);
}

int	ft_and_if(t_input *input, t_ast_node *root, int *fd)
{
	int		pid;
	int		st;
	int		status;

	st = 0;
	pid = 0;
	while (pid != -1)
		pid = waitpid(-1, &st, 0);
	while (*fd > 2)
		close((*fd)--);
	if (root->parent && root->parent->type == PIPELINE)
		status = ft_pipe(input, root, fd);
	else
		status = ft_command(root->data.command.cmd.args, input, root);
	return (status);
}

int	ft_or_if(t_input *input, t_ast_node *root, int *fd)
{
	int		pid;
	int		st;
	int		status;

	st = 0;
	pid = 0;
	status = 0;
	while (pid != -1)
		pid = waitpid(-1, &st, 0);
	while (*fd > 2)
		close((*fd)--);
	if (root->parent && root->parent->type == PIPELINE)
		status = ft_pipe(input, root, fd);
	else
		status = ft_command(root->data.command.cmd.args, input, root);
	return (status);
}
