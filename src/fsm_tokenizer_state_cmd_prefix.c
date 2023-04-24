/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fsm_tokenizer_state_cmd_prefix.c                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 16:39:23 by arobu             #+#    #+#             */
/*   Updated: 2023/04/24 20:10:58 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"
#include <sys/wait.h>
#include "signals.h"

static void	tokenizer_cmd_prefix_none(t_fsm *fsm, t_token *token);
static void	tokenizer_cmd_prefix_redir(t_fsm *fsm, \
			t_token *token, t_input *input);
static void	tokenizer_cmd_prefix_pipe_lop(t_token *token, t_fsm *fsm);

int	tokenizer_tok_cmd_prefix(t_fsm *fsm, t_token *token, t_input *input)
{
	if (is_token_redir(token) && fsm->cmd_p_substate == TOK_CMD_PREFIX_NONE)
		tokenizer_cmd_prefix_none(fsm, token);
	else if (fsm->cmd_p_substate == TOK_CMD_PREFIX_REDIR)
		tokenizer_cmd_prefix_redir(fsm, token, input);
	else if (is_assign_and_none(token, fsm))
		;
	else if (is_lop_and_none_literal(token, fsm))
		fsm->cmd_state = TOK_CMD_NAME;
	else if (is_rparen_and_none(token, fsm))
	{
		add_token(input->tokens, token);
		fsm->tok_state = TOK_RPARENTHESIS;
		return (1);
	}
	else if (is_lop_and_none(token, fsm))
		tokenizer_cmd_prefix_pipe_lop(token, fsm);
	else
	{
		fsm->state = ERROR;
		input->unexpected = token->type;
	}
	return (0);
}

static void	tokenizer_cmd_prefix_pipe_lop(t_token *token, t_fsm *fsm)
{
	if (token->type == TOKEN_PIPE)
		fsm->tok_state = TOK_PIPE;
	else if (token->type == TOKEN_AND_IF)
		fsm->tok_state = TOK_AND_IF;
	else if (token->type == TOKEN_OR_IF)
		fsm->tok_state = TOK_OR_IF;
}

static void	tokenizer_cmd_prefix_none(t_fsm *fsm, t_token *token)
{
	fsm->cmd_p_substate = TOK_CMD_PREFIX_REDIR;
	if (token->type == TOKEN_DLESS)
		fsm->redir_state = HEREDOC;
	else
		fsm->redir_state = OTHER;
}

static void	tokenizer_cmd_prefix_redir(t_fsm *fsm, \
		t_token *token, t_input *input)
{
	if (fsm->redir_state == OTHER)
	{
		if (!is_token_word_literal(token))
		{
			fsm->state = ERROR;
			input->unexpected = token->type;
		}
	}
	else if (fsm->redir_state == HEREDOC)
	{
		do_heredoc(token, fsm, input);
	}
	fsm->cmd_p_substate = TOK_CMD_PREFIX_NONE;
	fsm->redir_state = OTHER;
}
