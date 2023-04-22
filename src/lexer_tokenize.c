/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_tokenize.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 14:14:00 by arobu             #+#    #+#             */
/*   Updated: 2023/04/22 17:00:15 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

void	treat_squote(char c, int *squote, int *dquote, int *literal);
void	treat_dquote(char c, int *squote, int *dquote, int *literal);
void	init_word_parms(int *i, int *dquote, int *squote, int *literal);
void	advance_input(t_lexer *lexer, char *buffer, int *index);
int		is_correct_word(char c, int squote, int dquote);

t_token	*tokenize_word(t_lexer *lexer)
{
	char	buffer[100000];
	int		i;
	int		dquote;
	int		squote;
	int		literal;

	init_word_parms(&i, &dquote, &squote, &literal);
	ft_memset(buffer, '\0', 100000);
	while (is_correct_word(lexer->tok_ch, squote, dquote))
	{
		if ((lexer->input_len <= lexer->tok_position))
			break ;
		treat_squote(lexer->tok_ch, &squote, &dquote, &literal);
		treat_dquote(lexer->tok_ch, &squote, &dquote, &literal);
		advance_input(lexer, buffer, &i);
	}
	return (return_type(literal, buffer, lexer));
}

void	advance_input(t_lexer *lexer, char *buffer, int *index)
{
	(buffer)[*index] = lexer->tok_ch;
	get_next_char_tok(lexer);
	(*index)++;
}

int	is_correct_word(char c, int squote, int dquote)
{
	return (match_word(c) || c == '=' || dquote || squote);
}

void	init_word_parms(int *i, int *dquote, int *squote, int *literal)
{
	*i = 0;
	*dquote = 0;
	*squote = 0;
	*literal = 0;
}

void	treat_squote(char c, int *squote, int *dquote, int *literal)
{
	if (c == '\'' && !*dquote)
	{
		*squote ^= 1;
		*literal = 1;
	}
}

void	treat_dquote(char c, int *squote, int *dquote, int *literal)
{
	if (c == '\"' && !*squote)
	{
		*dquote ^= 1;
		*literal = 2;
	}
}

t_token	*return_type(int is_literal, char *buffer, t_lexer *lexer)
{
	lexer->tok_position--;
	if (is_literal == 1 && ft_strchr(buffer, '=') != NULL && \
		ft_strchr(buffer, '=') - ft_strchr(buffer, '\'') > 0)
		return (new_token(TOKEN_QUOTE, buffer, EMPTY));
	else if (is_literal == 1 && ft_strchr(buffer, '=') != NULL && \
		ft_strchr(buffer, '=') - ft_strchr(buffer, '\'') < 0)
		return (new_token(TOKEN_ASSIGN_WORD, buffer, EMPTY));
	else if (is_literal == 2 && ft_strchr(buffer, '=') != NULL && \
		ft_strchr(buffer, '=') - ft_strchr(buffer, '\"') > 0)
		return (new_token(TOKEN_QUOTE, buffer, EMPTY));
	else if (is_literal == 2 && ft_strchr(buffer, '=') != NULL && \
		ft_strchr(buffer, '=') - ft_strchr(buffer, '\"') < 0)
		return (new_token(TOKEN_ASSIGN_WORD, buffer, EMPTY));
	else if (is_literal == 1)
		return (new_token(TOKEN_QUOTE, buffer, EMPTY));
	else if (is_literal == 2)
		return (new_token(TOKEN_QUOTE, buffer, EMPTY));
	if (ft_strchr(buffer, '=') != NULL && buffer[0] != '=')
		return (new_token(TOKEN_ASSIGN_WORD, buffer, EMPTY));
	return (new_token(TOKEN_WORD, buffer, EMPTY));
}

t_token	*tokenize_pipe(t_lexer *lexer)
{
	if (look_ahead_tok(lexer) == '|')
	{
		get_next_char_tok(lexer);
		return (new_token(TOKEN_OR_IF, NULL, EMPTY));
	}
	return (new_token(TOKEN_PIPE, NULL, EMPTY));
}

t_token	*tokenize_ampersand(t_lexer *lexer)
{
	if (look_ahead_tok(lexer) == '&')
	{
		get_next_char_tok(lexer);
		return (new_token(TOKEN_AND_IF, NULL, EMPTY));
	}
	return (new_token(TOKEN_AND, NULL, EMPTY));
}

t_token	*tokenize_redir_input(t_lexer *lexer)
{
	t_redir_type	redir_type;

	redir_type = EMPTY;
	if (look_ahead_tok(lexer) == '<')
	{
		get_next_char_tok(lexer);
		return (new_token(TOKEN_DLESS, NULL, redir_type));
	}
	return (new_token(TOKEN_LESS, NULL, redir_type));
}

t_token	*tokenize_redir_output(t_lexer *lexer)
{
	t_redir_type	redir_type;

	redir_type = EMPTY;
	if (lexer->tok_position != 0)
	{
		if (lexer->input[lexer->tok_position - 1] == '0')
			redir_type = STD_IN;
		else if (lexer->input[lexer->tok_position - 1] == '1')
			redir_type = STD_OUT;
		else if (lexer->input[lexer->tok_position - 1] == '2')
			redir_type = STD_ERR;
	}
	if (look_ahead_tok(lexer) == '>')
	{
		get_next_char_tok(lexer);
		return (new_token(TOKEN_DGREAT, NULL, redir_type));
	}
	if (look_ahead_tok(lexer) == '|')
		return (new_token(TOKEN_GREAT, NULL, redir_type));
	return (new_token(TOKEN_GREAT, NULL, redir_type));
}
