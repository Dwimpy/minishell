/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fsm_tokenizer_state_pipe.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 16:25:37 by arobu             #+#    #+#             */
/*   Updated: 2023/04/24 16:48:37 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"
#include <sys/wait.h>
#include "signals.h"

int	tokenizer_do_pipe(t_token *token, t_fsm *fsm, t_input *input)
{
	if (token->type == TOKEN_LPARENTHESIS)
		;
	else if (is_token_redir(token))
	{
		fsm->tok_state = TOK_CMD;
		fsm->cmd_state = TOK_CMD_PREFIX;
		fsm->cmd_p_substate = TOK_CMD_PREFIX_REDIR;
	}
	else if (is_token_word_literal(token))
	{
		fsm->tok_state = TOK_CMD;
		fsm->cmd_state = TOK_CMD_PREFIX;
		fsm->cmd_p_substate = TOK_CMD_PREFIX_NONE;
	}
	else
	{
		fsm->state = ERROR;
		input->unexpected = token->type;
	}
	return (0);
}
