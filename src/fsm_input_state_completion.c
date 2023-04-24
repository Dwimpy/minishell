/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fsm_input_state_completion.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/23 21:15:59 by arobu             #+#    #+#             */
/*   Updated: 2023/04/24 20:42:14 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"
#include <sys/wait.h>
#include "signals.h"

void	get_the_input(t_input *input, t_fsm *fsm)
{
	t_lexer		*lexer;

	lexer = &input->lexer;
	fsm->input_state = N_INPUT;
	while (fsm->input_state != INPUT_COMPLETE)
	{
		if (fsm->state == GET_INPUT)
			run_fsm_input_get_input(lexer, fsm, input);
		else if (fsm->state == INCOMPLETE)
			do_fsm_incomplete_state(lexer, fsm, input);
	}
}

void	run_fsm_input_get_input(t_lexer *lexer, t_fsm *fsm, t_input *input)
{
	char	c;

	c = get_next_char(lexer);
	if (fsm->input_state == N_INPUT)
		fsm_input_state_update(c, lexer, fsm);
	else if (fsm->input_state == IN_LINEBR)
		do_linebreak(lexer, "> ", fsm);
	else if (fsm->input_state == IN_SQUOTE)
		do_squote(lexer, fsm);
	else if (fsm->input_state == IN_DQUOTE)
		do_dquote(lexer, fsm);
	else if (fsm->input_state == IN_SUBSH)
		do_subsh(input, fsm);
}
