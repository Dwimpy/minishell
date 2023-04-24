/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkilling <tkilling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/05 23:24:13 by dwimpy            #+#    #+#             */
/*   Updated: 2023/04/24 12:51:27 by tkilling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H
# include "token_list.h"
# include "readline/readline.h"
# include "readline/history.h"
// # include <readline/readline.h>
// # include <readline/history.h>
# include "hashmap.h"
# include "ast.h"
# include "get_next_line.h"

typedef struct s_lexer
{
	char				*input;
	int					read_position;
	int					tok_position;
	int					input_len;
	char				ch;
	char				tok_ch;
}						t_lexer;

typedef struct s_input
{
	t_token_list		*tokens;
	t_lexer				lexer;
	t_hashmap			*hashmap;
	t_hashmap			*special_sym;
	t_ast_node			*root;
	t_arglist			*heredoc_files;
    struct sigaction	sa;
	int					unexpected;
	int					sub;
}				t_input;

typedef struct s_control
{
	int					is_double_quote;
	int					is_single_quote;
	int					parens;
	int					parens_counter;
	char				*history;
}						t_control;

typedef enum e_incomplete_type
{
	NEW_LINE,
	SQUOTE,
	DQUOTE,
	SUBSH
}						t_incomplete_type;

int				init_lexer(t_lexer	*lexer);
int				append_input_pipe(t_lexer *lexer, t_token_type type);
int				append_to_input(t_lexer *lexer, t_incomplete_type type, \
					char **curr_history);
int				append_to_input_special(t_lexer *lexer, \
					t_incomplete_type type, char **curr_history);
void			get_new_history(char *append_input, t_incomplete_type type, \
					char **curr_history);
char			*str_join_newline(char *s1, char *s2);
int				match_whitespace(char c);
int				match_digit(char c);
int				match_symbol(char c);
int				match_letter(char c);
int				match_character(char c);
int				match_word(char c);
char			look_ahead(t_lexer *lexer);
char			look_ahead_tok(t_lexer *lexer);
char			get_next_char(t_lexer *lexer);
char			get_next_char_tok(t_lexer *lexer);
t_token			*create_next_token(t_lexer *lexer);
t_token			*tokenize_word(t_lexer *lexer);
t_token			*tokenize_pipe(t_lexer *lexer);
t_token			*tokenize_ampersand(t_lexer *lexer);
t_token			*tokenize_redir_input(t_lexer *lexer);
t_token			*tokenize_redir_output(t_lexer *lexer);
t_token			*tokenize_braces(t_lexer *lexer);
t_token			*tokenize_semicolon(t_lexer *lexer);
t_token			*tokenize_newline(t_lexer *lexer);
void			get_tokens(t_token_list *tokens, t_lexer lexer);
t_token			*return_type(int is_literal, char *buffer, t_lexer *lexer);
int				analyze_input(t_lexer *lexer);
t_control		init_control_structure(void);
int				is_incomplete_input(t_control control, char c);
void			handle_incomplete_input(t_control *control, char c);
char			*get_new_input(t_incomplete_type type);
int				get_new_lexer_input(t_lexer *lexer, t_control *control);
t_hashmap		*create_special_table(void);

#endif
