/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/06 19:26:01 by arobu             #+#    #+#             */
/*   Updated: 2023/04/22 21:24:43 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENIZER_H
# define TOKENIZER_H

# include "lexer.h"
# include "fsm.h"
# include "x_execution.h"

int		gen_input(t_input *input);
void	tokenize(t_input *input, t_fsm *fsm);
void	get_the_input(t_input *input, t_fsm *fsm);
void	do_linebreak(t_lexer *lexer, char *prompt, t_fsm *fsm);
void	do_squote(t_lexer *lexer, t_fsm *fsm);
void	do_dquote(t_lexer *lexer, t_fsm *fsm);
void	do_subsh(t_input *input, t_fsm *fsm);
int		do_in_pipe(t_lexer *lexer, t_fsm *fsm);
int		do_in_cmdand(t_lexer *lexer, t_fsm *fsm);
int		do_in_cmdor(t_lexer *lexer, t_fsm *fsm);
int		is_valid_beginning(t_token *token);
int		is_prefix(t_token *token);
int		is_cmd_suffix(t_token *token);
int		is_token_redir(t_token *token);
int		is_tokenizer_ending(t_input	*input);
int		is_token_word(t_token *token);
int		is_token_assignment(t_token *token);
int		is_token_word_literal(t_token *token);
int		is_token_logical_op(t_token *token);
int		is_tok_state_pipe_lop(t_fsm *fsm);
void	readline_new_line(t_lexer *lexer, char *prompt);
int		readline_no_new_line(t_lexer *lexer, char *prompt);
int		readline_pipe(t_lexer *lexer, char *prompt);
char	*get_prompt_dir(void);
char	*read_from_stdin(t_input *input);
void	fsm_input_state_update(char c, t_lexer *lexer, t_fsm *fsm);
void	print_syntax_error(int unexpected);
#endif
