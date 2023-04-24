/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fsm_tokenizer_state_cmd_suffix.c                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 17:45:19 by arobu             #+#    #+#             */
/*   Updated: 2023/04/24 18:20:29 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"
#include <sys/wait.h>
#include "signals.h"

static void	tokenizer_is_heredoc(t_token *token, t_fsm *fsm, t_input *input);
static void	tokenizer_suffix_lop(t_token *token, t_fsm *fsm);
static void	tokenizer_suffix_redir(t_token *token, t_fsm *fsm, t_input *input);

int	tokenizer_tok_cmd_suffix(t_token *token, \
		t_fsm *fsm, t_input *input)
{
	if (is_token_redir_and_none(token, fsm))
		tokenizer_is_heredoc(token, fsm, input);
	else if (fsm->cmd_p_substate == TOK_CMD_SUFFIX_REDIR)
		tokenizer_suffix_redir(token, fsm, input);
	else if (is_token_word_literal(token) || is_token_assignment(token))
		;
	else if (is_token_logical_op(token))
		tokenizer_suffix_lop(token, fsm);
	else if (is_token_rparen(token))
	{
		add_token(input->tokens, token);
		fsm->tok_state = TOK_RPARENTHESIS;
		return (1);
	}
	else
	{
		fsm->state = ERROR;
		input->unexpected = token->type;
	}
	return (0);
}

static void	tokenizer_suffix_redir(t_token *token, t_fsm *fsm, t_input *input)
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
	fsm->cmd_p_substate = TOK_CMD_SUFFIX_NONE;
	fsm->redir_state = OTHER;
}

static void	tokenizer_suffix_lop(t_token *token, t_fsm *fsm)
{
	if (token->type == TOKEN_PIPE)
		fsm->tok_state = TOK_PIPE;
	else if (token->type == TOKEN_AND_IF)
		fsm->tok_state = TOK_AND_IF;
	else if (token->type == TOKEN_OR_IF)
		fsm->tok_state = TOK_OR_IF;
}

static void	tokenizer_is_heredoc(t_token *token, t_fsm *fsm, t_input *input)
{
	fsm->cmd_p_substate = TOK_CMD_SUFFIX_REDIR;
	if (token->type == TOKEN_DLESS)
		fsm->redir_state = HEREDOC;
	else
		fsm->redir_state = OTHER;
}
