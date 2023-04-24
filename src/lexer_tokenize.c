/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_tokenize.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 14:14:00 by arobu             #+#    #+#             */
/*   Updated: 2023/04/24 22:29:40 by arobu            ###   ########.fr       */
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
