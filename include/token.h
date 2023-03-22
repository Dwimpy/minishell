/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 12:22:38 by arobu             #+#    #+#             */
/*   Updated: 2023/03/22 19:11:22 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_H
# define TOKEN_H
# include <stdlib.h>
# include <stdio.h>
# include "ft_printf.h"

typedef struct s_token	t_token;

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_SQUOTE,
	TOKEN_DQUOTE,
	TOKEN_ASSIGN_WORD,
	TOKEN_PIPE,	
	TOKEN_OR_IF,
	TOKEN_AND,
	TOKEN_AND_IF,
	TOKEN_LESS,
	TOKEN_GREAT,
	TOKEN_DLESS,
	TOKEN_DGREAT,
	TOKEN_LPARENTHESIS,
	TOKEN_RPARENTHESIS,
	TOKEN_EOF
}						t_token_type;

typedef struct s_token_word
{
	char			*value;
}						t_token_word;

typedef struct s_token_squote
{
	char			*value;
}						t_token_squote;

typedef struct s_token_dquote
{
	char			*value;
}						t_token_dquote;

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
	char			c;
}				t_token_great;

typedef struct s_token_dless
{
	char			*value;
}				t_token_dless;

typedef struct s_token_dgreat
{
	char			*value;
}				t_token_dgreat;

typedef struct s_token_linebreak
{
	char			c;
}				t_token_linebreak;

typedef struct s_token_lbrace
{
	char			c;
}				t_token_lparantesis;

typedef struct s_token_rbrace
{
	char			c;
}				t_token_rparantesis;

typedef struct s_token_eof
{
	char			c;
}				t_token_eof;

typedef union u_token_value
{
	t_token_word			word;
	t_token_squote			squote;
	t_token_dquote			dquote;
	t_token_assign_word		assign_word;
	t_token_pipe			pipe;
	t_token_and				and;
	t_token_or_if			or_if;
	t_token_and_if			and_if;
	t_token_less			less;
	t_token_great			great;
	t_token_dless			dless;
	t_token_dgreat			dgreat;
	t_token_linebreak		linebreak;
	t_token_lparantesis		lparanthesis;
	t_token_rparantesis		rparanthesis;
	t_token_eof				eof;
}						t_token_value;

typedef struct s_token
{
	t_token_type		type;
	t_token_value		value;
	t_token				*next;
}						t_token;

t_token		*new_token(t_token_type type, char *value);
int			is_string_type(t_token_type type);
int			is_operator_type(t_token_type type);
t_token		*return_type(int is_literal, char *buffer);

void		print_token_value(t_token *token);
void		free_token(t_token *token);
#endif
