/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 12:22:38 by arobu             #+#    #+#             */
/*   Updated: 2023/04/25 17:35:51 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_H
# define TOKEN_H
# include <stdlib.h>
# include <stdio.h>
# include "quote_list.h"
# include "ft_printf.h"

typedef struct s_token	t_token;

typedef enum e_token_type
{
	TOKEN_WORD = 1,
	TOKEN_QUOTE = 2,
	TOKEN_ASSIGN_WORD = 3,
	TOKEN_PIPE = 4,
	TOKEN_OR_IF = 5,
	TOKEN_AND = 6,
	TOKEN_AND_IF = 7,
	TOKEN_LESS = 8,
	TOKEN_GREAT = 9,
	TOKEN_DLESS = 10,
	TOKEN_DGREAT = 11,
	TOKEN_LPARENTHESIS = 12,
	TOKEN_RPARENTHESIS = 13,
	TOKEN_SEMICOLON = 14,
	TOKEN_UNEXPECTED = 15,
	TOKEN_NEWLINE = 16,
	TOKEN_EOF = 17,
	TOKEN_END_OF_FILE = 18
}						t_token_type;

typedef enum e_redir_type
{
	STD_IN,
	STD_OUT,
	STD_ERR,
	EMPTY
}				t_redir_type;

typedef struct s_token_word
{
	char			*value;
}						t_token_word;

typedef struct s_token_quote
{
	char			*value;
	t_quotelist		*quotes;
}						t_token_quote;

typedef struct s_token_assign_word
{
	char			*value;
}						t_token_assign_word;

typedef struct s_token_pipe
{
	char			c;
}				t_token_pipe;

typedef struct s_token_or_if
{
	char			*value;
}				t_token_or_if;

typedef struct s_token_and
{
	char			c;
}				t_token_and;

typedef struct s_token_and_if
{
	char			*value;
}				t_token_and_if;

typedef struct s_token_less
{
	char			c;
}				t_token_less;

typedef struct s_token_great
{
	char			*symbol;
	t_redir_type	from;
}				t_token_great;

typedef struct s_token_dless
{
	char			*value;
}				t_token_dless;

typedef struct s_token_dgreat
{
	char			*value;
	t_redir_type	from;
}				t_token_dgreat;

typedef struct s_token_lbrace
{
	char			c;
}				t_token_lparantesis;

typedef struct s_token_rbrace
{
	char			c;
}				t_token_rparantesis;

typedef struct s_token_no_match
{
	char			*value;
}				t_token_no_match;

typedef struct s_token_eof
{
	char			c;
}				t_token_eof;

typedef struct s_token_newline
{
	char			c;
}				t_token_newline;

typedef struct s_token_semicolon
{
	char			c;
}				t_token_semicolon;

typedef union u_token_value
{
	t_token_word			word;
	t_token_quote			quote;
	t_token_assign_word		assign_word;
	t_token_pipe			pipe;
	t_token_and				and;
	t_token_or_if			or_if;
	t_token_and_if			and_if;
	t_token_less			less;
	t_token_great			great;
	t_token_dless			dless;
	t_token_dgreat			dgreat;
	t_token_newline			new_line;
	t_token_lparantesis		lparanthesis;
	t_token_rparantesis		rparanthesis;
	t_token_semicolon		semicolon;
	t_token_eof				eof;
}						t_token_value;

typedef struct s_token
{
	t_token_type		type;
	t_token_value		value;
	t_token				*next;
}						t_token;

t_token		*new_token(t_token_type type, char *value, t_redir_type redir_type);
int			is_token_lparen(t_token *token);
int			is_token_rparen(t_token *token);
int			is_token_pipe(t_token *token);
int			is_token_cmdand(t_token *token);
int			is_token_cmdor(t_token *token);
int			is_string_type(t_token_type type);
int			is_operator_type(t_token_type type);
void		print_token_value(t_token *token);
void		free_token(t_token *token);
#endif
