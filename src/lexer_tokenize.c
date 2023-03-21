/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_tokenize.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 14:14:00 by arobu             #+#    #+#             */
/*   Updated: 2023/03/21 14:56:52 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

t_token	*tokenize_word(t_lexer *lexer)
{
	char	buffer[4096];
	int		i;
	int		in_double_quote;
	int		in_single_quote;
	int		is_literal;

	i = 0;
	in_double_quote = 0;
	in_single_quote = 0;
	is_literal = 0;
	while (match_word(lexer->ch) || lexer->ch == '=' \
		|| in_single_quote || in_double_quote)
	{
		if (lexer->input_len == lexer->read_position)
			break ;
		if (lexer->ch == '\'' && !in_double_quote)
		{
			in_single_quote ^= 1;
			is_literal = 1;
		}
		if (lexer->ch == '\"' && !in_single_quote)
		{
			in_double_quote ^= 1;
			is_literal = 2;
		}
		buffer[i] = lexer->ch;
		get_next_char(lexer);
		i++;
	}
	lexer->read_position--;
	buffer[i] = '\0';
	return (return_type(is_literal, buffer));
}

t_token	*return_type(int is_literal, char *buffer)
{
	if (is_literal == 1 && ft_strchr(buffer, '=') != NULL && \
		ft_strchr(buffer, '=') - ft_strchr(buffer, '\'') > 0)
		return (new_token(TOKEN_SQUOTE, buffer));
	else if (is_literal == 1 && ft_strchr(buffer, '=') != NULL && \
		ft_strchr(buffer, '=') - ft_strchr(buffer, '\'') < 0)
		return (new_token(TOKEN_ASSIGN_WORD, buffer));
	else if (is_literal == 2 && ft_strchr(buffer, '=') != NULL && \
		ft_strchr(buffer, '=') - ft_strchr(buffer, '\"') > 0)
		return (new_token(TOKEN_DQUOTE, buffer));
	else if (is_literal == 2 && ft_strchr(buffer, '=') != NULL && \
		ft_strchr(buffer, '=') - ft_strchr(buffer, '\"') < 0)
		return (new_token(TOKEN_ASSIGN_WORD, buffer));
	else if (is_literal == 1)
		return (new_token(TOKEN_SQUOTE, buffer));
	else if (is_literal == 2)
		return (new_token(TOKEN_DQUOTE, buffer));
	if (ft_strchr(buffer, '=') != NULL && buffer[0] != '=')
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
