/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_create_free_file.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 02:00:57 by arobu             #+#    #+#             */
/*   Updated: 2023/04/25 02:13:51 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"
#include "parser.h"
#include "x_execution.h"

int	open_file(char **filename, int io, t_input *input);

void	create_and_free(t_token *token, \
		char **filename, int io, t_input *input)
{
	int	fd;

	if (!*filename)
	{
		if (token->type == TOKEN_QUOTE)
			*filename = get_env_vars(\
				expand_vars(token->value.quote.value), input);
		else if (token->type == TOKEN_WORD)
			*filename = get_env_vars(\
				expand_vars(token->value.word.value), input);
	}
	else
	{
		free(*filename);
		if (token->type == TOKEN_QUOTE)
			*filename = get_env_vars(\
				expand_vars(token->value.quote.value), input);
		else if (token->type == TOKEN_WORD)
			*filename = get_env_vars(\
				expand_vars(token->value.word.value), input);
	}
	fd = open_file(filename, io, input);
	if (fd < 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(*filename, 2);
		ft_putendl_fd(": No such file or directory", 2);
	}
	close(fd);
}

int	open_file(char **filename, int io, t_input *input)
{
	int	fd;

	fd = 0;
	if (io == INPUT)
	{
		if (!ft_strncmp(*filename, "./", 2))
			fd = open(&(*filename)[2], O_RDONLY);
		else
			fd = open((*filename), O_RDONLY);
	}
	else if (io == HERE_DOC)
	{
		fd = open(*filename, O_CREAT | O_WRONLY | O_APPEND, 0644);
		if (fd > 0)
			new_argument(input->heredoc_files, create_heredoc_file(*filename));
	}
	else if (io == OUTPUT)
	{
		if (!ft_strncmp(*filename, "./", 2))
			fd = open(*filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		else
			fd = open(*filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	}
	else if (io == OUTPUT_APPEND)
	{
		if (!ft_strncmp(*filename, "./", 2))
			fd = open(*filename, O_CREAT | O_WRONLY | O_APPEND, 0644);
		else
			fd = open(*filename, O_CREAT | O_WRONLY | O_APPEND, 0644);
	}
	return (fd);
}
