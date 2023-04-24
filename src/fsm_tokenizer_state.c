/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fsm_tokenizer_state.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/23 21:17:12 by arobu             #+#    #+#             */
/*   Updated: 2023/04/24 18:33:45 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"
#include <sys/wait.h>
#include "signals.h"

int	is_running(t_fsm *fsm);
int	is_beginning(t_token *token, t_fsm *fsm, t_input *input);

void	tokenize(t_input *input, t_fsm *fsm)
{
	t_lexer			*lexer;
	t_token			*token;
	t_token_list	*tokens;
	int				status;

	if (init_state_get_tokens_substates(fsm))
		return ;
	init_state_get_tokens(&lexer, &tokens, &token, input);
	status = 0;
	while (is_running(fsm))
	{
		token = create_next_token(lexer);
		status = run_tokenizer(token, fsm, input);
		if (status == 2)
			return ;
		else if (status == 1)
			continue ;
		add_token(tokens, token);
	}
}

int	run_tokenizer(t_token *token, t_fsm *fsm, t_input *input)
{
	if (end_of_tokens(token, input->tokens, fsm, input))
		return (2);
	else
		tokenizer_run_n_tok_state(token, fsm, input);
	if (fsm->tok_state == TOK_CMD)
	{
		if (tokenizer_do_tok_cmd(token, fsm, input))
			return (1);
	}
	else if (is_tok_state_pipe_lop(fsm))
		tokenizer_do_pipe(token, fsm, input);
	else if (fsm->tok_state == TOK_LPARENTHESIS)
		tokenizer_do_lparen(token, fsm, input);
	else if (fsm->tok_state == TOK_RPARENTHESIS)
		if (tokenizer_do_rparen(token, fsm, input))
			return (1);
	return (0);
}

int	end_of_tokens(t_token *token, t_token_list *tokens, \
				t_fsm *fsm, t_input *input)
{
	if (!token)
	{
		if (!tokens->last)
			fsm->state = COMPLETE;
		else if (is_tokenizer_ending(input))
			do_tokenizer_ending(fsm, tokens, input);
		else
			tokenizer_input_complete(fsm);
		return (1);
	}
	return (0);
}

void	tokenizer_input_complete(t_fsm *fsm)
{
	fsm->tok_state = TOK_COMPLETE;
	fsm->state = COMPLETE;
}

void	do_tokenizer_ending(t_fsm *fsm, t_token_list *tokens, t_input *input)
{
	fsm->state = INCOMPLETE;
	if (tokens->last->type == TOKEN_PIPE)
		fsm->tok_state = TOK_PIPE;
	else if (tokens->last->type == TOKEN_AND_IF)
		fsm->tok_state = TOK_AND_IF;
	else if (tokens->last->type == TOKEN_OR_IF)
		fsm->tok_state = TOK_OR_IF;
	else if (is_token_redir(tokens->last))
	{
		fsm->state = ERROR;
		input->unexpected = TOKEN_EOF;
	}
}
