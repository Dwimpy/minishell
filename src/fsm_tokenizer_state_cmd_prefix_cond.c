/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fsm_tokenizer_state_cmd_prefix_cond.c              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 17:41:20 by arobu             #+#    #+#             */
/*   Updated: 2023/04/24 17:43:33 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"
#include <sys/wait.h>
#include "signals.h"

int	is_lop_and_none(t_token *token, t_fsm *fsm)
{
	return (is_token_logical_op(token) && \
			fsm->cmd_state == TOK_CMD_PREFIX_NONE);
}

int	is_lop_and_none_literal(t_token *token, t_fsm *fsm)
{
	return (is_token_word_literal(token) && \
		fsm->cmd_p_substate == TOK_CMD_PREFIX_NONE);
}

int	is_rparen_and_none(t_token *token, t_fsm *fsm)
{
	return (is_token_rparen(token) && \
	fsm->cmd_p_substate == TOK_CMD_PREFIX_NONE);
}

int	is_assign_and_none(t_token *token, t_fsm *fsm)
{
	return (is_token_assignment(token) && \
		fsm->cmd_p_substate == TOK_CMD_PREFIX_NONE);
}
