/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_subshell.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/23 21:31:40 by arobu             #+#    #+#             */
/*   Updated: 2023/04/23 21:42:15 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"
#include <sys/wait.h>
#include "signals.h"

static int	incorrect_parenthesis(t_input *input, t_fsm *fsm);
static int	subshell_end_of_input(t_input *input, t_fsm * fsm);
static void	subshell_states_update(t_lexer *lexer, t_fsm *fsm);

void	do_subsh(t_input *input, t_fsm *fsm)
{
	t_lexer	*lexer;

	lexer = &input->lexer;
	if (incorrect_parenthesis(input, fsm))
		return ;
	else if (subshell_end_of_input(input, fsm))
		return ;
	subshell_states_update(lexer, fsm);
}

void	subshell_states_update(t_lexer *lexer, t_fsm *fsm)
{
	if (lexer->ch == '\'')
		fsm->input_state = IN_SQUOTE;
	else if (lexer->ch == '\"')
		fsm->input_state = IN_DQUOTE;
	else if (lexer->ch == ')' && fsm->paren > 0)
	{
		fsm->input_state = N_INPUT;
		fsm->paren -= 1;
	}
	else if (lexer->ch == ')')
	{
		fsm->input_state = N_INPUT;
		fsm->paren -= 1;
		fsm->in_subshell = 0;
	}
	else if (lexer->ch == '(')
	{
		fsm->paren += 1;
		fsm->input_state = N_INPUT;
	}
}

static int	subshell_end_of_input(t_input *input, t_fsm * fsm)
{
	if (input->lexer.ch == '\0')
	{
		if (readline_no_new_line(&input->lexer, "subsh> "))
		{
			fsm->input_state = INPUT_COMPLETE;
			fsm->state = ERROR;
			input->unexpected = TOKEN_END_OF_FILE;
			return (1);
		}
	}
	return (0);
}

static int	incorrect_parenthesis(t_input *input, t_fsm *fsm)
{
	if (fsm->paren < 0)
	{
		fsm->input_state = INPUT_COMPLETE;
		fsm->state = ERROR;
		input->unexpected = TOKEN_RPARENTHESIS;
		return (1);
	}
	return (0);
}
