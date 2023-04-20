/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   x_subshell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 15:20:28 by tkilling          #+#    #+#             */
/*   Updated: 2023/04/20 02:22:04 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "x_execution.h"

int ft_subshell_no_tree(t_input *input, t_ast_node *root)
{
	int	pid;
	int	status;

	pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0)
	{
		if (root->is_subshell > 1)
			exit(1);
		status = ft_command(root->data.command.cmd.args, input, root, 0);
		exit(status);
	}
	else
	{
		waitpid(pid, &status, 0);
		return (WEXITSTATUS(status));
	}
}
