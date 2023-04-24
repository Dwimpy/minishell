/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   x_subshell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkilling <tkilling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 15:20:28 by tkilling          #+#    #+#             */
/*   Updated: 2023/04/24 10:06:23 by tkilling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "x_execution.h"
#include "signals.h"

int	ft_subshell_no_tree(t_input *input, t_ast_node *root)
{
	int	pid;
	int	status;

	pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0)
	{
		ft_signals_child(&(input->sa));
		if (root->is_subshell > 1)
			exit(1);
		status = ft_command(root->data.command.cmd.args, input, root);
		exit(status);
	}
	else
	{
		waitpid(pid, &status, 0);
		return (WEXITSTATUS(status));
	}
}

int	ft_add_subshell(t_input *input, t_ast_node *root, int *fd, int subshell)
{
	static int		new_fd[2];
	int				pid;
	int				status;
	t_ast_node		*ptr;

	status = 0;
	ptr = root;
	while (ptr && ptr->is_subshell >= subshell)
		ptr = ptr->parent;
	if (pipe(new_fd) == -1)
		return (-1);
	pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0)
	{
		ft_signals_child(&(input->sa));
		close(new_fd[0]);
		if (ptr && ptr->type == PIPELINE)
			dup2(new_fd[1], STDOUT_FILENO);
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
		return (WEXITSTATUS(status));
	}
}
