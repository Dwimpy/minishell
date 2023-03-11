/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_tokenize.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 14:14:00 by arobu             #+#    #+#             */
/*   Updated: 2023/03/11 21:20:14 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

t_token	*tokenize_word(t_lexer *lexer)
{
	char	buffer[4096];
	int		i;

	i = 0;
	while (match_word(lexer->ch))
	{
		buffer[i] = lexer->ch;
		get_next_char(lexer);
		i++;
	}
	lexer->read_position--;
	buffer[i] = '\0';
	return (new_token(TOKEN_WORD, \
		(t_token_value){.string = ft_strdup(buffer)}));
}

t_token	*tokenize_pipe(t_lexer *lexer)
{
	char	buffer[3];

	buffer[0] = lexer->ch;
	buffer[1] = '\0';
	if (look_ahead(lexer) == '|')
	{
		get_next_char(lexer);
		buffer[1] = lexer->ch;
		buffer[2] = '\0';
		return (new_token(TOKEN_OR, \
			(t_token_value){.string = ft_strdup(buffer)}));
	}
	return (new_token(TOKEN_PIPE, \
		(t_token_value){.operator = buffer[0]}));
}

t_token	*tokenize_ampersand(t_lexer *lexer)
{
	char	buffer[3];

	buffer[0] = lexer->ch;
	buffer[1] = '\0';
	if (look_ahead(lexer) == '&')
	{
		get_next_char(lexer);
		buffer[1] = lexer->ch;
		buffer[2] = '\0';
		return (new_token(TOKEN_D_AND, \
			(t_token_value){.string = ft_strdup(buffer)}));
	}
	return (new_token(TOKEN_AND, \
		(t_token_value){.operator = buffer[0]}));
}

t_token	*tokenize_redir_input(t_lexer *lexer)
{
	char	buffer[3];

	buffer[0] = lexer->ch;
	buffer[1] = '\0';
	if (look_ahead(lexer) == '<')
	{
		get_next_char(lexer);
		buffer[1] = lexer->ch;
		buffer[2] = '\0';
		return (new_token(TOKEN_REDIR_INPUT_APPEND, \
			(t_token_value){.string = ft_strdup(buffer)}));
	}
	return (new_token(TOKEN_REDIR_INPUT, \
			(t_token_value){.operator = buffer[0]}));
}

t_token	*tokenize_redir_output(t_lexer *lexer)
{
	char	buffer[3];

	buffer[0] = lexer->ch;
	buffer[1] = '\0';
	if (look_ahead(lexer) == '>')
	{
		get_next_char(lexer);
		buffer[1] = lexer->ch;
		buffer[2] = '\0';
		return (new_token(TOKEN_REDIR_INPUT_APPEND, \
			(t_token_value){.string = ft_strdup(buffer)}));
	}
	return (new_token(TOKEN_REDIR_INPUT, \
			(t_token_value){.operator = buffer[0]}));
}
