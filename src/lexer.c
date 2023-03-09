/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dwimpy <dwimpy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/06 16:59:45 by dwimpy            #+#    #+#             */
/*   Updated: 2023/03/08 22:34:02 by dwimpy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include <stdio.h>
t_lexer	*init_lexer(char *input)
{
	t_lexer *lexer;
	
	lexer = (t_lexer *)malloc(sizeof(t_lexer));
	lexer->input = ft_strtrim(input ," \t\n");
	lexer->position = 0;
	lexer->read_position = 0;
	lexer->input_len = ft_strlen(lexer->input);
	lexer->ch = '\0';
	return (lexer);
}

char	get_next_char(t_lexer *lexer)
{
	lexer->read_position++;
	if (lexer->read_position >= lexer->input_len)
	{
		lexer->ch = '\0';
		return (lexer->ch);
	}
	else
		lexer->ch = lexer->input[lexer->read_position];
	return (lexer->ch);
}

t_token	*get_next_token(t_lexer *lexer)
{
	t_token *token;
	char	*str;
		
	skip_whitespace(lexer);
	while (match_word(get_next_char(lexer)))
		;
	str = ft_substr(lexer->input, lexer->position, lexer->read_position - lexer->position);
	token = new_token(str);
	lexer->position = lexer->read_position;
	return (token);
}

void skip_whitespace(t_lexer *lexer)
{
	while (match_whitespace(lexer->ch))
	{
		lexer->position++;
		get_next_char(lexer);
	}
}

int	match_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

int	match_word(char c)
{
	return (ft_isalnum(c) || c == '_' || c == '\"');
}

t_token	*new_token(char *value)
{
	t_token *token;
	
	token = (t_token *)malloc(sizeof(t_token));
	token->value = value;
	token->type = TOKEN_EOF;
	token->next = NULL;
	return (token);
}