/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fsm_tokenizer_state_paren.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 16:16:16 by arobu             #+#    #+#             */
/*   Updated: 2023/04/24 16:27:35 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"
#include <sys/wait.h>
#include "signals.h"

int	tokenizer_do_rparen(t_token *token, t_fsm *fsm, t_input *input)
{
	if (is_token_logical_op(token))
	{
		if (token->type == TOKEN_PIPE)
			fsm->tok_state = TOK_PIPE;
		else if (token->type == TOKEN_AND_IF)
			fsm->tok_state = TOK_AND_IF;
		else if (token->type == TOKEN_OR_IF)
			fsm->tok_state = TOK_OR_IF;
	}
	else if (is_token_rparen(token))
	{
		add_token(input->tokens, token);
		return (1);
	}
	else
	{
		fsm->state = ERROR;
		input->unexpected = token->type;
	}	
	return (0);
}

int	tokenizer_do_lparen(t_token *token, t_fsm *fsm, t_input *input)
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
