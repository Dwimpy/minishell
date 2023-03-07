/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dwimpy <dwimpy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/05 23:24:13 by dwimpy            #+#    #+#             */
/*   Updated: 2023/03/07 16:18:55 by dwimpy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

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
	const char  *input;
	int         position;
	int         read_position;
	char        ch;
}				t_lexer;

t_lexer *init_lexer(char *input);
char	 get_next_char(t_lexer *lexer);

#endif