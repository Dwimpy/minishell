/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fsm.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/06 17:18:46 by arobu             #+#    #+#             */
/*   Updated: 2023/04/06 20:35:30 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FSM_H
# define FSM_H

typedef enum s_parse_state
{
	GET_INPUT,
	GET_TOKENS,
	INCOMPLETE,
	COMPLETE,
	ERROR
}				t_parse_state;

typedef enum e_input_state
{
	N_INPUT,
	IN_LINEBR,
	IN_SQUOTE,
	IN_DQUOTE,
	INPUT_COMPLETE
}				t_input_state;

typedef enum e_tokenizer_state
{
	N_TOKENIZER,
	IN_CMD,
	IN_LPARENTHESIS,
	IN_RPARENTHESIS,
	IN_PIPE,
	IN_AND_IF,
	IN_OR_IF,
}				t_tokenizer_state;

typedef enum e_cmd_substate
{
	IN_CMD_PREFIX,
	IN_CMD_NAME,
	IN_CMD_SUFFIX
}				t_cmd_substate;

typedef struct s_fsm
{
	int					in_paren;
	t_parse_state		state;
	t_input_state		input_state;
	t_tokenizer_state	tok_state;
}					t_fsm;

#endif
