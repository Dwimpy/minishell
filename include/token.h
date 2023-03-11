/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 12:22:38 by arobu             #+#    #+#             */
/*   Updated: 2023/03/11 21:14:19 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_H
# define TOKEN_H
# include <stdlib.h>
# include <stdio.h>

typedef struct s_token	t_token;

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_OR,
	TOKEN_AND,
	TOKEN_D_AND,
	TOKEN_REDIR_INPUT,
	TOKEN_REDIR_OUTPUT,
	TOKEN_REDIR_INPUT_APPEND,
	TOKEN_REDIR_OUTPUT_APPEND,
	TOKEN_BACKGROUND,
	TOKEN_EOF
}						t_token_type;

typedef union u_token_value
{
	char				*string;
	char				operator;
}						t_token_value;

typedef struct s_token
{
	t_token_type		type;
	t_token_value		value;
	t_token				*next;
}						t_token;

t_token		*new_token(t_token_type type, t_token_value value);
t_token		*new_empty_token(void);
int			is_string_type(t_token_type type);
int			is_operator_type(t_token_type type);
void		print_token_value(t_token *token);
void		free_token(t_token *token);
#endif
