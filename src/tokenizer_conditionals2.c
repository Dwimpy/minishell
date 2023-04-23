/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_conditionals2.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/23 18:49:29 by arobu             #+#    #+#             */
/*   Updated: 2023/04/23 18:50:24 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"
#include <sys/wait.h>
#include "signals.h"

inline int	is_token_logical_op(t_token *token)
{
	return (token->type == TOKEN_AND_IF || \
		token->type == TOKEN_OR_IF || \
		token->type == TOKEN_PIPE);
}

inline int	is_tok_state_pipe_lop(t_fsm *fsm)
{
	return (fsm->tok_state == TOK_PIPE || \
		fsm->tok_state == TOK_AND_IF || \
		fsm->tok_state == TOK_OR_IF);
}

inline int is_token_word(t_token *token)
{
	return (token->type == TOKEN_WORD);
}

inline int	is_token_word_literal(t_token *token)
{
	return (token->type == TOKEN_WORD || token->type == TOKEN_QUOTE || \
		token->type == TOKEN_QUOTE);
}

inline int	is_token_redir(t_token *token)
{
	return (token->type == TOKEN_LESS || token->type == TOKEN_GREAT || \
		token->type == TOKEN_DLESS || token->type == TOKEN_DGREAT);
}
