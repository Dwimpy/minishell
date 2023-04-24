/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   x_redirect.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkilling <tkilling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 13:17:07 by tkilling          #+#    #+#             */
/*   Updated: 2023/04/24 13:34:16 by tkilling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "x_execution.h"
#include "sys/types.h"
#include "sys/wait.h"
#include "sys/stat.h"

int	ft_re_stdout(int *stdout_cp, int fd)
{
	if (fd < 0)
		return (1);
	*stdout_cp = dup(STDOUT_FILENO);
	close(STDOUT_FILENO);
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

int	ft_re_stdin(int *stdin_cp, int fd)
{
	if (fd < 0)
		return (1);
	*stdin_cp = dup(STDIN_FILENO);
	close(STDIN_FILENO);
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}

int	ft_redirect(t_ast_node *root, int *stdin_cp, int *stdout_cp)
{
	int		fd;

	if (root->data.command.output.filename
		&& !root->data.command.output.is_appended)
	{
		fd = open(root->data.command.output.filename, O_WRONLY | O_TRUNC, 0644);
		if (ft_re_stdout(stdout_cp, fd) == 1)
			return (1);
	}
	else if (root->data.command.output.filename
		&& root->data.command.output.is_appended)
	{
		fd = open(root->data.command.output.filename,
				O_WRONLY | O_APPEND, 0644);
		if (ft_re_stdout(stdout_cp, fd) == 1)
			return (1);
	}
	if (root->data.command.input.filename)
	{
		fd = open(root->data.command.input.filename, O_RDONLY);
		if (ft_re_stdout(stdin_cp, fd) == 1)
			return (1);
	}
	return (0);
}

void	ft_redirect_back(int *stdin_cp, int *stdout_cp)
{	
	if (*stdout_cp >= 0)
	{
		close (STDOUT_FILENO);
		dup2(*stdout_cp, STDOUT_FILENO);
		close(*stdout_cp);
	}
	if (*stdin_cp >= 0)
	{
		close (STDIN_FILENO);
		dup2(*stdin_cp, STDIN_FILENO);
		close(*stdin_cp);
	}
}
