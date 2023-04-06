/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_analyzer.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 12:32:57 by arobu             #+#    #+#             */
/*   Updated: 2023/04/06 17:19:43 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SYNTAX_ANALYZER_H
# define SYNTAX_ANALYZER_H

# include "token_list.h"
# include "fsm.h"

int		analyze_syntax(t_token_list *tokens, int *unexpected);
void	update_state(t_token *token, t_fsm *fsm);
int		state_in_cmd(t_fsm *fsm, t_token **token, int *unexpected);
int		state_in_pipe(t_fsm *fsm, t_token **token, int *unexpected);
int		state_in_op(t_fsm *fsm, t_token **token, int *unexpected);
int		state_in_lp(t_fsm *fsm, t_token **token, int *unexpected);
int		state_in_rp(t_fsm *fsm, t_token **token, int *unexpected);
int		is_left_parenthesis(t_token *token);
int		is_right_parenthesis(t_token *token);
int		is_token_eof(t_token *token);
int		is_valid_beginning(t_token *token);
int		accept_prefix(t_token **token, t_fsm *state);
int		accept_cmd_word(t_token **token, t_fsm *state);
int		accept_cmd_suffix(t_token **token, t_fsm *state);
int		accept_pipe(t_token **token, t_fsm *state);
int		accept_logical_op(t_token **token, t_fsm *state);
int		accept_left_parenthesis(t_token **token, t_fsm *state);
int		accept_right_parenthesis(t_token **token, t_fsm *state);
#endif
