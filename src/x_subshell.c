/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   x_subshell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 15:20:28 by tkilling          #+#    #+#             */
/*   Updated: 2023/04/23 21:30:24 by arobu            ###   ########.fr       */
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
