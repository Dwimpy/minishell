/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fsm.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/06 17:18:46 by arobu             #+#    #+#             */
/*   Updated: 2023/04/10 22:47:29 by arobu            ###   ########.fr       */
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
	IN_SUBSH,
	INPUT_COMPLETE
}				t_input_state;

typedef enum e_tokenizer_state
{
	N_TOKENIZER,
	TOK_CMD,
	TOK_LPARENTHESIS,
	TOK_RPARENTHESIS,
	TOK_PIPE,
	TOK_AND_IF,
	TOK_OR_IF,
	TOK_COMPLETE
}				t_tokenizer_state;

typedef enum e_cmd_substate
{
	TOK_CMD_PREFIX,
	TOK_CMD_NAME,
	TOK_CMD_SUFFIX,
}				t_cmd_substate;

typedef enum e_cmd_p_substates
{
	TOK_CMD_PREFIX_NONE,
	TOK_CMD_NAME_NONE,
	TOK_CMD_PREFIX_REDIR,
	TOK_CMD_SUFFIX_NONE,
	TOK_CMD_SUFFIX_REDIR
}				t_cmd_p_substates;

typedef struct s_fsm
{
	t_parse_state			state;
	t_input_state			input_state;
	t_tokenizer_state		tok_state;
	t_cmd_substate			cmd_state;
	t_cmd_p_substates		cmd_p_substate;
}					t_fsm;

#endif
