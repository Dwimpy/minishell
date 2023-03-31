/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_analyzer_states.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 17:33:47 by arobu             #+#    #+#             */
/*   Updated: 2023/03/31 17:34:25 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	state_in_rp(t_fsm *fsm, t_token **token, int *unexpected)
{
	if (fsm->state == IN_RPARENTHESIS)
	{
		if (accept_right_parenthesis(token, fsm) == TOKEN_UNEXPECTED)
			return (*unexpected = (*token)->next->type);
	}
	return (0);
}

int	state_in_lp(t_fsm *fsm, t_token **token, int *unexpected)
{
	if (fsm->state == IN_LPARENTHESIS)
	{
		if (accept_left_parenthesis(token, fsm) == TOKEN_UNEXPECTED)
			return (*unexpected = (*token)->next->type);
	}
	return (0);
}

int	state_in_op(t_fsm *fsm, t_token **token, int *unexpected)
{
	if (fsm->state == IN_AND_IF || fsm->state == IN_OR_IF)
	{
		if (accept_logical_op(token, fsm) == TOKEN_UNEXPECTED)
			return (*unexpected = (*token)->next->type);
	}
	return (0);
}

int	state_in_pipe(t_fsm *fsm, t_token **token, int *unexpected)
{
	if (fsm->state == IN_PIPE)
	{
		if (accept_pipe(token, fsm) == TOKEN_UNEXPECTED)
			return (*unexpected = (*token)->next->type);
	}
	return (0);
}

int	state_in_cmd(t_fsm *fsm, t_token **token, int *unexpected)
{
	if (fsm->state == IN_CMD)
	{
		if (fsm->substate == IN_CMD_PREFIX && \
				accept_prefix(token, fsm) == TOKEN_UNEXPECTED)
			return (*unexpected = (*token)->next->type);
		else if (fsm->substate == IN_CMD && \
				accept_cmd_word(token, fsm) == TOKEN_UNEXPECTED)
			return (*unexpected = (*token)->next->type);
		else if (fsm->substate == IN_CMD_SUFFIX && \
				accept_cmd_suffix(token, fsm) == TOKEN_UNEXPECTED)
			return (*unexpected = (*token)->next->type);
	}
	return (0);
}
