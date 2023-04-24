/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_acceptor.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 09:57:56 by arobu             #+#    #+#             */
/*   Updated: 2023/04/24 22:13:02 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	accept(t_token *token, t_token_type type)
{
	return (token->type == type);
}

int	expect(t_token *token, t_token_type type)
{
	return (accept(token, type));
}

int	accept_redirection(t_token *token)
{
	if (accept(token, TOKEN_LESS) || accept(token, TOKEN_GREAT) || \
	accept(token, TOKEN_DGREAT) || accept(token, TOKEN_DLESS))
	{
		if (is_cmd_word(token->next))
			return (1);
	}
	return (0);
}

int	is_cmd_word(t_token *token)
{
	return (accept(token, TOKEN_WORD) || \
		accept(token, TOKEN_QUOTE));
}
