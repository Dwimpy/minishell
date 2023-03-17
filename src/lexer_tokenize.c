/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_tokenize.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 14:14:00 by arobu             #+#    #+#             */
/*   Updated: 2023/03/17 16:41:14 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

t_token	*tokenize_word(t_lexer *lexer)
{
	char	buffer[4096];
	int		i;

	i = 0;
	while (match_word(lexer->ch) || lexer->ch == '=')
	{
		buffer[i] = lexer->ch;
		get_next_char(lexer);
		i++;
	}
	lexer->read_position--;
	buffer[i] = '\0';
	if (ft_strchr(buffer, '=') != NULL)
		return (new_token(TOKEN_ASSIGN_WORD, buffer));
	return (new_token(TOKEN_WORD, buffer));
}

t_token	*tokenize_pipe(t_lexer *lexer)
{
	if (look_ahead(lexer) == '|')
	{
		get_next_char(lexer);
		return (new_token(TOKEN_OR_IF, NULL));
	}
	return (new_token(TOKEN_PIPE, NULL));
}

t_token	*tokenize_ampersand(t_lexer *lexer)
{
	if (look_ahead(lexer) == '&')
	{
		get_next_char(lexer);
		return (new_token(TOKEN_AND_IF, NULL));
	}
	return (new_token(TOKEN_AND, NULL));
}

t_token	*tokenize_redir_input(t_lexer *lexer)
{
	if (look_ahead(lexer) == '<')
	{
		get_next_char(lexer);
		return (new_token(TOKEN_DLESS, NULL));
	}
	return (new_token(TOKEN_LESS, NULL));
}

t_token	*tokenize_redir_output(t_lexer *lexer)
{
	if (look_ahead(lexer) == '>')
	{
		get_next_char(lexer);
		return (new_token(TOKEN_DGREAT, NULL));
	}
	return (new_token(TOKEN_GREAT, NULL));
}
