/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/05 23:24:13 by dwimpy            #+#    #+#             */
/*   Updated: 2023/04/01 17:46:15 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H
# include "token_list.h"
# include "ft_printf.h"
# include <readline/history.h>
# include <readline/readline.h>

typedef struct s_lexer
{
	char				*input;
	int					read_position;
	int					input_len;
	char				ch;
}						t_lexer;

int				init_lexer(t_lexer *lexer);
int				append_to_input(t_lexer *lexer, \
					char *prompt, char **curr_history);
char			*str_join_newline(char *s1, char *s2);
char			*add_new_line_at_end(char *s1);
int				match_whitespace(char c);
int				match_digit(char c);
int				match_symbol(char c);
int				match_letter(char c);
int				match_character(char c);
int				match_word(char c);
char			look_ahead(t_lexer *lexer);
char			get_next_char(t_lexer *lexer);
t_token			*create_next_token(t_lexer *lexer);
t_token			*tokenize_word(t_lexer *lexer);
t_token			*tokenize_pipe(t_lexer *lexer);
t_token			*tokenize_ampersand(t_lexer *lexer);
t_token			*tokenize_redir_input(t_lexer *lexer);
t_token			*tokenize_redir_output(t_lexer *lexer);
t_token			*tokenize_braces(t_lexer *lexer);
void			get_tokens(t_token_list *tokens, t_lexer lexer);
t_token			*return_type(int is_literal, char *buffer, t_lexer *lexer);
int				analyze_input(t_lexer *lexer);

#endif
