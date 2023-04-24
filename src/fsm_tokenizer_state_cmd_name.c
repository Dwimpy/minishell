/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fsm_tokenizer_state_cmd_name.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 18:20:56 by arobu             #+#    #+#             */
/*   Updated: 2023/04/24 18:25:12 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"
#include <sys/wait.h>
#include "signals.h"

static void	tokenizer_suffix_cmd_name_redir(t_token *token, t_fsm *fsm);

int	tokenizer_tok_cmd_name(t_token *token, \
		t_fsm *fsm, t_input *input)
{
	if (is_token_word_literal(token) || is_token_assignment(token))
		;
	else if (is_token_redir(token))
		tokenizer_suffix_cmd_name_redir(token, fsm);
	else if (is_token_logical_op(token))
	{
		add_token(input->tokens, token);
		if (token->type == TOKEN_PIPE)
			fsm->tok_state = TOK_PIPE;
		else if (token->type == TOKEN_AND_IF)
			fsm->tok_state = TOK_AND_IF;
		else if (token->type == TOKEN_OR_IF)
			fsm->tok_state = TOK_OR_IF;
		return (1);
	}
	else if (is_token_rparen(token))
		fsm->tok_state = TOK_RPARENTHESIS;
	else
	{
		fsm->state = ERROR;
		input->unexpected = token->type;
	}
	return (0);
}

static void	tokenizer_suffix_cmd_name_redir(t_token *token, t_fsm *fsm)
{
	fsm->cmd_state = TOK_CMD_SUFFIX;
	fsm->cmd_p_substate = TOK_CMD_SUFFIX_REDIR;
	if (token->type == TOKEN_DLESS)
	{
		fsm->redir_state = HEREDOC;
	}
	else
		fsm->redir_state = OTHER;
}
