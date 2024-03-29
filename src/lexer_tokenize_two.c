/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_tokenize_two.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 18:58:12 by arobu             #+#    #+#             */
/*   Updated: 2023/04/24 22:29:53 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

t_token	*tokenize_braces(t_lexer *lexer)
{
	if (lexer->tok_ch == '(')
		return (new_token(TOKEN_LPARENTHESIS, NULL, EMPTY));
	if (lexer->tok_ch == ')')
		return (new_token(TOKEN_RPARENTHESIS, NULL, EMPTY));
	return (NULL);
}

t_token	*tokenize_newline(t_lexer *lexer)
{
	(void)lexer;
	return (new_token(TOKEN_NEWLINE, NULL, EMPTY));
}

t_token	*tokenize_semicolon(t_lexer *lexer)
{
	(void)lexer;
	return (new_token(TOKEN_SEMICOLON, NULL, EMPTY));
}

void	treat_dquote(char c, int *squote, int *dquote, int *literal)
{
	if (c == '\"' && !*squote)
	{
		*dquote ^= 1;
		*literal = 2;
	}
}
