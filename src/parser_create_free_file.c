/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_create_free_file.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 02:00:57 by arobu             #+#    #+#             */
/*   Updated: 2023/04/25 02:49:20 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"
#include "parser.h"
#include "x_execution.h"

static void	get_filename(t_token *token, char **filename, t_input *input);

void	create_and_free(t_token *token, \
		char **filename, int io, t_input *input)
{
	int	fd;

	get_filename(token, filename, input);
	fd = open_file(filename, io, input);
	if (fd < 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(*filename, 2);
		ft_putendl_fd(": No such file or directory", 2);
	}
	close(fd);
}

static void	get_filename(t_token *token, char **filename, t_input *input)
{
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
}
