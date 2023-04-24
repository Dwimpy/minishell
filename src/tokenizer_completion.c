/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_completion.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/23 18:56:36 by arobu             #+#    #+#             */
/*   Updated: 2023/04/24 20:11:27 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"
#include <sys/wait.h>
#include "signals.h"

int	do_in_pipe(t_lexer *lexer, t_fsm *fsm)
{
	if (readline_pipe(lexer, "pipe> "))
		return (1);
	fsm->state = GET_INPUT;
	fsm->input_state = N_INPUT;
	return (0);
}

int	do_in_cmdand(t_lexer *lexer, t_fsm *fsm)
{
	if (readline_pipe(lexer, "cmdand> "))
		return (1);
	fsm->state = GET_INPUT;
	fsm->input_state = N_INPUT;
	return (0);
}

int	do_in_cmdor(t_lexer *lexer, t_fsm *fsm)
{
	if (readline_pipe(lexer, "cmdor> "))
		return (1);
	fsm->state = GET_INPUT;
	fsm->input_state = N_INPUT;
	return (0);
}
