/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fsm_input_state.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/23 18:51:46 by arobu             #+#    #+#             */
/*   Updated: 2023/04/24 20:34:41 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"
#include <sys/wait.h>
#include "signals.h"

static void	fsm_input_state_completions(char c, t_lexer *lexer, t_fsm *fsm);
static void	fsm_set_init_input_state(char c, t_lexer *lexer, t_fsm *fsm);

void	fsm_input_state_update(char c, t_lexer *lexer, t_fsm *fsm)
{
	fsm_set_init_input_state(c, lexer, fsm);
	fsm_input_state_completions(c, lexer, fsm);
}

static void	fsm_set_init_input_state(char c, t_lexer *lexer, t_fsm *fsm)
{
	if (c == '\0' && !lexer->input_len)
		fsm->state = COMPLETE;
	else if (c == '\0' && fsm->paren < 0)
	{
		fsm->input_state = INPUT_COMPLETE;
		fsm->state = ERROR;
	}
	else if (c == '\0' && fsm->paren > 0)
		fsm->input_state = IN_SUBSH;
	else if (c == '\0')
		fsm->input_state = INPUT_COMPLETE;
}

static void	fsm_input_state_completions(char c, t_lexer *lexer, t_fsm *fsm)
{
	if (c == '\\' && look_ahead(lexer) == '\0')
		fsm->input_state = IN_LINEBR;
	if (c == '\'')
		fsm->input_state = IN_SQUOTE;
	if (c == '"')
		fsm->input_state = IN_DQUOTE;
	if (c == '(')
	{
		fsm->input_state = IN_SUBSH;
		fsm->paren += 1;
		fsm->in_subshell = 1;
	}
	if (c == ')')
	{
		fsm->input_state = IN_SUBSH;
		fsm->paren -= 1;
	}
}

int	is_empty(char *str)
{
	int	i;

	i = 0;
	while (match_whitespace(str[i]))
		i++;
	if (str[i] == '\0')
		return (1);
	return (0);
}
