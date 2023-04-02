/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_analyzer_acceptors.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 17:35:08 by arobu             #+#    #+#             */
/*   Updated: 2023/04/02 15:15:26 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	accept_pipe(t_token **token, t_fsm *state)
{
	if (is_pipe(*token))
	{
		if (is_prefix((*token)->next) || is_cmd_suffix((*token)->next))
		{
			(*token) = (*token)->next;
			state->state = NOTHING;
			return (0);
		}
		else if (is_left_parenthesis((*token)->next))
		{
			(*token) = (*token)->next;
			state->state = NOTHING;
			return (0);
		}
	}
	return (TOKEN_UNEXPECTED);
}

int	accept_cmd_suffix(t_token **token, t_fsm *state)
{
	if (is_input_redir(*token) || is_output_redir(*token))
	{
		if (is_cmd_word((*token)->next))
		{
			(*token) = (*token)->next;
			(*token) = (*token)->next;
			if (is_right_parenthesis(*token))
				state->state = NOTHING;
			return (0);
		}
	}
	else if (is_cmd_word(*token))
	{
		(*token) = (*token)->next;
		if (is_right_parenthesis(*token))
			state->state = NOTHING;
		return (0);
	}
	else if (is_pipe(*token) || is_logical_op(*token))
	{
		state->state = NOTHING;
		return (0);
	}
	return (TOKEN_UNEXPECTED);
}

int	accept_cmd_word(t_token **token, t_fsm *state)
{
	if (is_cmd_word(*token))
	{
		(*token) = (*token)->next;
		if (is_right_parenthesis(*token))
			state->state = NOTHING;
		return (0);
	}
	else if (is_input_redir(*token) || is_output_redir(*token))
	{
		state->substate = IN_CMD_SUFFIX;
		return (0);
	}
	else if (is_pipe(*token) || is_logical_op(*token))
	{
		state->state = NOTHING;
		return (0);
	}
	return (TOKEN_UNEXPECTED);
}

int	accept_prefix(t_token **token, t_fsm *state)
{
	if (is_input_redir(*token) || is_output_redir(*token))
	{
		if (is_cmd_word((*token)->next))
		{
			(*token) = (*token)->next;
			(*token) = (*token)->next;
			if (is_right_parenthesis(*token))
				state->state = NOTHING;
			return (0);
		}
	}
	else if (is_assign_word(*token))
	{
		(*token) = (*token)->next;
		if (is_right_parenthesis(*token))
			state->state = NOTHING;
		return (0);
	}
	else if (is_cmd_word(*token))
	{
		state->substate = IN_CMD_NAME;
		return (0);
	}
	else if (is_pipe(*token) || is_logical_op(*token))
	{
		state->state = NOTHING;
		return (0);
	}
	return (TOKEN_UNEXPECTED);
}

int	accept_logical_op(t_token **token, t_fsm *state)
{
	if (is_logical_op(*token))
	{
		if (is_prefix((*token)->next) || is_cmd_suffix((*token)->next))
		{
			(*token) = (*token)->next;
			state->state = NOTHING;
			return (0);
		}
		else if (is_left_parenthesis((*token)->next))
		{
			(*token) = (*token)->next;
			state->state = NOTHING;
			return (0);
		}
	}
	return (TOKEN_UNEXPECTED);
}
