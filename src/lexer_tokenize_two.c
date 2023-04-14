/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_tokenize_two.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 18:58:12 by arobu             #+#    #+#             */
/*   Updated: 2023/04/07 19:32:00 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

t_token	*tokenize_braces(t_lexer *lexer)
{
	if (lexer->tok_ch == '(')
		return (new_token(TOKEN_LPARENTHESIS, NULL));
	if (lexer->tok_ch == ')')
		return (new_token(TOKEN_RPARENTHESIS, NULL));
	return (NULL);
}

t_token	*tokenize_newline(t_lexer *lexer)
{
	return (new_token(TOKEN_NEWLINE, NULL));
}

t_token	*tokenize_semicolon(t_lexer *lexer)
{
	return (new_token(TOKEN_SEMICOLON, NULL));
}
