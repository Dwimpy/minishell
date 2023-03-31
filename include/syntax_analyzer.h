/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_analyzer.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 12:32:57 by arobu             #+#    #+#             */
/*   Updated: 2023/03/31 17:10:45 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SYNTAX_ANALYZER_H
# define SYNTAX_ANALYZER_H

# include "token_list.h"

typedef enum e_syntax_type
{
	NOTHING,
	IN_CMD,
	IN_LPARENTHESIS,
	IN_RPARENTHESIS,
	IN_PIPE,
	IN_AND_IF,
	IN_OR_IF
}				t_syntax_type;

typedef enum e_substate_type
{
	IN_CMD_PREFIX,
	IN_CMD_NAME,
	IN_CMD_SUFFIX
}				t_substate_type;

typedef struct s_fsm
{
	int				in_paren;
	t_syntax_type	state;
	t_substate_type	substate;
}					t_fsm;

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
