/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fsm_tokenizer_state_func.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/23 21:58:52 by arobu             #+#    #+#             */
/*   Updated: 2023/04/24 20:06:45 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"
#include <sys/wait.h>
#include "signals.h"

static int	is_beginning(t_token *token, t_fsm *fsm, t_input *input);

int	is_running(t_fsm *fsm)
{
	return (fsm->tok_state != TOK_COMPLETE && \
			fsm->state != INCOMPLETE && \
			fsm->state != COMPLETE && \
			fsm->state != ERROR);
}

void	init_state_get_tokens(t_lexer **lexer, t_token_list **tokens, \
			t_token **token, t_input *input)
{
	*lexer = &input->lexer;
	*token = NULL;
	*tokens = input->tokens;
}

int	init_state_get_tokens_substates(t_fsm *fsm)
{
	if (fsm->state == ERROR)
		return (1);
	fsm->state = GET_TOKENS;
	fsm->tok_state = N_TOKENIZER;
	return (0);
}

int	tokenizer_run_n_tok_state(t_token *token, t_fsm *fsm, t_input *input)
{
	if (is_beginning(token, fsm, input))
		return (1);
	if (fsm->tok_state == N_TOKENIZER)
	{
		if (is_prefix(token) || is_cmd_suffix(token))
		{
			fsm->tok_state = TOK_CMD;
			fsm->cmd_state = TOK_CMD_PREFIX;
			fsm->cmd_p_substate = TOK_CMD_PREFIX_NONE;
			fsm->redir_state = OTHER;
		}
		else if (is_token_lparen(token))
			fsm->tok_state = TOK_LPARENTHESIS;
		else if (is_token_rparen(token))
			fsm->tok_state = TOK_RPARENTHESIS;
		else if (is_token_pipe(token))
			fsm->tok_state = TOK_PIPE;
		else if (is_token_cmdand(token))
			fsm->tok_state = TOK_AND_IF;
		else if (is_token_cmdor(token))
			fsm->tok_state = TOK_OR_IF;
	}
	return (0);
}

static int	is_beginning(t_token *token, t_fsm *fsm, t_input *input)
{
	if (input->tokens->num_tokens == 0)
	{
		if (!is_valid_beginning(token))
		{
			fsm->state = ERROR;
			input->unexpected = token->type;
			return (1);
		}
	}
	return (0);
}
