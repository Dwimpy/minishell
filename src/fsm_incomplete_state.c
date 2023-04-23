/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fsm_incomplete_state.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/23 18:40:18 by arobu             #+#    #+#             */
/*   Updated: 2023/04/23 18:40:31 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"
#include <sys/wait.h>
#include "signals.h"

void	do_fsm_incomplete_state(t_lexer *lexer, t_fsm *fsm, t_input *input)
{
	if (fsm->tok_state == TOK_PIPE)
		do_fsm_incomplete_pipe(lexer, fsm, input);
	else if (fsm->tok_state == TOK_AND_IF)
		do_fsm_incomplete_cmd_and(lexer, fsm, input);
	else if (fsm->tok_state == TOK_OR_IF)
		do_fsm_incomplete_cmd_or(lexer, fsm, input);
}

void	do_fsm_incomplete_pipe(t_lexer *lexer, t_fsm *fsm, t_input *input)
{
	if (do_in_pipe(lexer, fsm))
		set_fsm_error(fsm, input);
}

void	do_fsm_incomplete_cmd_and(t_lexer *lexer, t_fsm *fsm, t_input *input)
{
	if (do_in_cmdand(lexer, fsm))
		set_fsm_error(fsm, input);
}

void	do_fsm_incomplete_cmd_or(t_lexer *lexer, t_fsm *fsm, t_input *input)
{
	if (do_in_cmdor(lexer, fsm))
		set_fsm_error(fsm, input);
}

void	set_fsm_error(t_fsm *fsm, t_input *input)
{
	fsm->input_state = INPUT_COMPLETE;
	fsm->state = ERROR;
	input->unexpected = TOKEN_END_OF_FILE;
}
