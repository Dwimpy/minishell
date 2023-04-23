/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_conditionals3.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/23 18:49:56 by arobu             #+#    #+#             */
/*   Updated: 2023/04/23 18:50:55 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"
#include <sys/wait.h>
#include "signals.h"

inline int	is_token_cmdor(t_token *token)
{
	return (token->type == TOKEN_OR_IF);
}

inline int	is_tokenizer_ending(t_input	*input)
{
	return (input->tokens->last->type != TOKEN_WORD && \
			input->tokens->last->type != TOKEN_SEMICOLON && \
			input->tokens->last->type != TOKEN_QUOTE && \
			input->tokens->last->type != TOKEN_RPARENTHESIS && \
			input->tokens->last->type != TOKEN_ASSIGN_WORD);
}
