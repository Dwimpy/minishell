// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   syntax_analyzer_acceptors_2.c                      :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2023/03/31 17:36:02 by arobu             #+#    #+#             */
// /*   Updated: 2023/03/31 22:40:27 by arobu            ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

// #include "parser.h"

// int	accept_right_parenthesis(t_token **token, t_fsm *state)
// {
// 	if (is_right_parenthesis(*token))
// 	{
// 		if (is_pipe((*token)->next) || is_logical_op((*token)->next))
// 		{
// 			state->in_paren -= 1;
// 			(*token) = (*token)->next;
// 			state->state = NOTHING;
// 			return (0);
// 		}
// 		if (is_right_parenthesis((*token)->next))
// 		{
// 			(*token) = (*token)->next;
// 			state->in_paren -= 1;
// 			state->state = NOTHING;
// 			return (0);
// 		}
// 		if (is_token_eof((*token)->next) && state->in_paren == 1)
// 		{
// 			(*token) = (*token)->next;
// 			return (0);
// 		}
// 	}
// 	return (TOKEN_UNEXPECTED);
// }

// int	accept_left_parenthesis(t_token **token, t_fsm *state)
// {
// 	if (is_left_parenthesis(*token))
// 	{
// 		state->in_paren += 1;
// 		if (is_prefix((*token)->next) || is_cmd_suffix((*token)->next))
// 		{
// 			(*token) = (*token)->next;
// 			state->state = NOTHING;
// 			return (0);
// 		}
// 	}
// 	return (TOKEN_UNEXPECTED);
// }
