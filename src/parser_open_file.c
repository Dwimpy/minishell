/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_open_file.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 02:43:04 by arobu             #+#    #+#             */
/*   Updated: 2023/04/25 17:02:59 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"
#include "parser.h"
#include "x_execution.h"

static int	open_fd_heredoc(char *filename, t_input *input);
static int	open_fd_output(char *filename);
static int	open_fd_output_append(char *filename);
static int	open_fd_input(char *filename);

int	open_file(char **filename, int io, t_input *input)
{
	int	fd;

	fd = 0;
	if (io == INPUT)
		fd = open_fd_input(*filename);
	else if (io == HERE_DOC)
		fd = open_fd_heredoc(*filename, input);
	else if (io == OUTPUT)
		fd = open_fd_output(*filename);
	else if (io == OUTPUT_APPEND)
		fd = open_fd_output_append(*filename);
	return (fd);
}

static int	open_fd_output_append(char *filename)
{
	int	fd;

	if (!ft_strncmp(filename, "./", 2))
		fd = open(filename, O_CREAT | O_WRONLY | O_APPEND, 0644);
	else
		fd = open(filename, O_CREAT | O_WRONLY | O_APPEND, 0644);
	return (fd);
}

static int	open_fd_heredoc(char *filename, t_input *input)
{
	int	fd;

	fd = open(filename, O_CREAT | O_WRONLY | O_APPEND, 0644);
	if (fd > 0)
		new_argument(input->heredoc_files, create_heredoc_file(filename));
	return (fd);
}

static int	open_fd_output(char *filename)
{
	int	fd;

	fd = -1;
	if (!ft_strncmp(filename, "./", 2))
		fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	else
		fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	return (fd);
}

static int	open_fd_input(char *filename)
{
	int	fd;

	fd = 0;
	if (!ft_strncmp(filename, "./", 2))
		fd = open(&(filename)[2], O_RDONLY);
	else
		fd = open(filename, O_RDONLY);
	return (fd);
}
