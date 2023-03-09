/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dwimpy <dwimpy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/05 23:24:13 by dwimpy            #+#    #+#             */
/*   Updated: 2023/03/08 20:57:05 by dwimpy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H
# include "ft_printf.h"
typedef struct s_token  t_token;

typedef enum s_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_INPUT,
	TOKEN_REDIR_OUTPUT,
	TOKEN_EOF
}				t_token_type;

typedef struct s_token
{
	char        *value;
	t_token_type type;
	t_token     *next;
}				t_token;

typedef struct s_lexer
{
	const char	*input;
	size_t		position;
	size_t		read_position;
	size_t		input_len;
	char		ch;
}				t_lexer;

t_lexer	*init_lexer(char *input);
t_token	*new_token(char *value);
int		match_word(char c);
int		match_whitespace(char c);
void 	skip_whitespace(t_lexer *lexer);
char	 get_next_char(t_lexer *lexer);
t_token	*get_next_token(t_lexer *lexer);
#endif