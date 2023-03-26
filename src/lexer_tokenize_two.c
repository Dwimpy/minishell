/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_tokenize_two.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 18:58:12 by arobu             #+#    #+#             */
/*   Updated: 2023/03/26 15:14:45 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

t_token	*tokenize_braces(t_lexer *lexer)
{
	if (lexer->ch == '(')
		return (new_token(TOKEN_LPARENTHESIS, NULL));
	if (lexer->ch == ')')
		return (new_token(TOKEN_RPARENTHESIS, NULL));
	return (NULL);
}
