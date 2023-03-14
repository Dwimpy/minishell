/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/06 16:59:45 by dwimpy            #+#    #+#             */
/*   Updated: 2023/03/14 19:05:53 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include <stdio.h>

void	skip_whitespace(t_lexer *lexer);

void	init_lexer(t_lexer *lexer, char *input)
{
	lexer->input = ft_strtrim(input, " \t\n");
	lexer->read_position = -1;
	lexer->input_len = ft_strlen(lexer->input);
	lexer->ch = '\0';
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
	char	buffer[4096];
	int		i;

	i = 0;
	get_next_char(lexer);
	if (lexer->ch == '\0')
		return (new_token(TOKEN_EOF, NULL));
	if (match_word(lexer->ch))
		return (tokenize_word(lexer));
	if (lexer->ch == '|')
		return (tokenize_pipe(lexer));
	if (lexer->ch == '&')
		return (tokenize_ampersand(lexer));
	if (lexer->ch == '<')
		return (tokenize_redir_input(lexer));
	if (lexer->ch == '>')
		return (tokenize_redir_output(lexer));
	if (lexer->ch == '(' || lexer->ch == ')')
		return (tokenize_braces(lexer));
	return (NULL);
}

char	look_ahead(t_lexer *lexer)
{
	if (lexer->read_position < lexer->input_len)
		return (lexer->input[lexer->read_position + 1]);
	else
		return ('\0');
}
