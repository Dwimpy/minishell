// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   syntax_analyzer.c                                  :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2023/03/23 13:43:59 by arobu             #+#    #+#             */
// /*   Updated: 2023/04/02 22:28:38 by arobu            ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

// #include "parser.h"

// int	analyze_syntax(t_token_list *tokens, int *unexpected)
// {
// 	t_fsm	fsm;
// 	t_token	*token;

// 	token = tokens->first;
// 	if (!token)
// 		return (0);
// 	fsm.state = NOTHING;
// 	fsm.in_paren = 0;
// 	if (!is_valid_beginning(token) && token->type != TOKEN_EOF)
// 		return (*unexpected = token->next->type);
// 	while (token->type != TOKEN_EOF)
// 	{
// 		if (fsm.state == NOTHING)
// 			update_state(token, &fsm);
// 		if (state_in_cmd(&fsm, &token, unexpected))
// 			return (*unexpected);
// 		if (state_in_pipe(&fsm, &token, unexpected))
// 			return (*unexpected);
// 		if (state_in_op(&fsm, &token, unexpected))
// 			return (*unexpected);
// 		if (state_in_lp(&fsm, &token, unexpected))
// 			return (*unexpected);
// 		if (state_in_rp(&fsm, &token, unexpected))
// 			return (*unexpected);
// 	}
// 	return (0);
// }

// void	update_state(t_token *token, t_fsm *fsm)
// {
// 	if (is_prefix(token) || is_cmd_suffix(token))
// 	{
// 		fsm->state = IN_CMD;
// 		fsm->substate = IN_CMD_PREFIX;
// 	}
// 	else if (token->type == TOKEN_LPARENTHESIS)
// 		fsm->state = IN_LPARENTHESIS;
// 	else if (token->type == TOKEN_RPARENTHESIS)
// 		fsm->state = IN_RPARENTHESIS;
// 	else if (token->type == TOKEN_PIPE)
// 		fsm->state = IN_PIPE;
// 	else if (token->type == TOKEN_AND_IF)
// 		fsm->state = IN_AND_IF;
// 	else if (token->type == TOKEN_OR_IF)
// 		fsm->state = IN_OR_IF;
// }

// int	is_valid_beginning(t_token *token)
// {
// 	return (is_cmd_word(token) || \
// 			is_input_redir(token) || \
// 				is_output_redir(token) || \
// 					token->type == TOKEN_LPARENTHESIS || \
// 						token->type == TOKEN_ASSIGN_WORD);
// }
