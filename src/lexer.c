/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/06 16:59:45 by dwimpy            #+#    #+#             */
/*   Updated: 2023/04/23 18:18:53 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include "lexer.h"
#include "fsm.h"


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

char	get_next_char_tok(t_lexer *lexer)
{
	lexer->tok_position++;
	if (lexer->tok_position >= lexer->input_len)
	{
		lexer->tok_ch = '\0';
		return (lexer->tok_ch);
	}
	else
		lexer->tok_ch = lexer->input[lexer->tok_position];
	return (lexer->tok_ch);
}


t_token	*create_next_token(t_lexer *lexer)
{
	while (match_whitespace(get_next_char_tok(lexer)))
		;
	if ((lexer->tok_ch == '1' || lexer->tok_ch == '2' || lexer->tok_ch == '0') && \
		look_ahead_tok(lexer) == '>')
		get_next_char_tok(lexer);
	if (match_word(lexer->tok_ch))
		return (tokenize_word(lexer));
	else if (lexer->tok_ch == '|')
		return (tokenize_pipe(lexer));
	else if (lexer->tok_ch == '&')
		return (tokenize_ampersand(lexer));
	else if (lexer->tok_ch == '<')
		return (tokenize_redir_input(lexer));
	else if (lexer->tok_ch == '>')
		return (tokenize_redir_output(lexer));
	else if (lexer->tok_ch == '(' || lexer->tok_ch == ')')
		return (tokenize_braces(lexer));
	else if (lexer->tok_ch == ';')
		return (tokenize_semicolon(lexer));
	if (lexer->ch == '\n')
		return (tokenize_newline(lexer));
	return (NULL);
}

char	look_ahead(t_lexer *lexer)
{
	if (lexer->read_position < lexer->input_len)
		return (lexer->input[lexer->read_position + 1]);
	else
		return ('\0');
}

char	look_ahead_tok(t_lexer *lexer)
{
	if (lexer->tok_position < lexer->input_len)
		return (lexer->input[lexer->tok_position + 1]);
	else
		return ('\0');
}
