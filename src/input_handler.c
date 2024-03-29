/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/18 12:33:51 by arobu             #+#    #+#             */
/*   Updated: 2023/04/07 14:17:31 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token_list.h"
#include "lexer.h"

void	get_tokens(t_token_list *tokens, t_lexer lexer)
{
	t_token	*token;

	while (lexer.tok_position < lexer.input_len)
	{
		token = create_next_token(&lexer);
		add_token(tokens, token);
	}
}
