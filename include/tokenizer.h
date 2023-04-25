/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/06 19:26:01 by arobu             #+#    #+#             */
/*   Updated: 2023/04/25 17:36:04 by arobu            ###   ########.fr       */
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
void	do_fsm_incomplete_state(t_lexer *lexer, t_fsm *fsm, t_input *input);
void	do_fsm_incomplete_pipe(t_lexer *lexer, t_fsm *fsm, t_input *input);
void	do_fsm_incomplete_cmd_and(t_lexer *lexer, t_fsm *fsm, t_input *input);
void	do_fsm_incomplete_cmd_or(t_lexer *lexer, t_fsm *fsm, t_input *input);
void	set_fsm_error(t_fsm *fsm, t_input *input);
void	run_fsm_input_get_input(t_lexer *lexer, t_fsm *fsm, t_input *input);
void	print_syntax_error(int unexpected);
void	fsm_init_tokenizer_params(int *index, int *status, \
		pid_t *pid, char **prompt);
int		get_tokenizer_input(t_input *input);
void	free_input_at_exit(t_input *input);
void	free_input_on_error(t_input *input);
int		handle_tokenizer_errors(t_input *input, t_fsm *fsm);
int		is_empty(char *str);
void	init_state_get_tokens(t_lexer **lexer, t_token_list **tokens, \
			t_token **token, t_input *input);
int		init_state_get_tokens_substates(t_fsm *fsm);
int		tokenizer_run_n_tok_state(t_token *token, t_fsm *fsm, t_input *input);
int		end_of_tokens(t_token *token, t_token_list *tokens, \
			t_fsm *fsm, t_input *input);
void	tokenizer_input_complete(t_fsm *fsm);
void	do_tokenizer_ending(t_fsm *fsm, t_token_list *tokens, t_input *input);
int		do_heredoc(t_token *token, t_fsm *fsm, t_input *input);
int		check_correct_token(t_token *token, t_fsm *fsm, t_input *input);
void	get_file_name(t_token *token, t_input *input, char **filename);
int		heredoc_token_word(char *line, t_token *token, t_input *input, int fd);
int		heredoc_token_quote(char *line, t_token *token, t_input *input, int fd);
int		run_heredoc(t_token *token, t_input *input, int *fd, char *line);
int		tokenizer_do_rparen(t_token *token, t_fsm *fsm, t_input *input);
int		tokenizer_do_lparen(t_token *token, t_fsm *fsm, t_input *input);
int		tokenizer_do_pipe(t_token *token, t_fsm *fsm, t_input *input);
int		tokenizer_tok_cmd_prefix(t_fsm *fsm, t_token *token, t_input *input);
int		is_lop_and_none(t_token *token, t_fsm *fsm);
int		is_lop_and_none_literal(t_token *token, t_fsm *fsm);
int		is_rparen_and_none(t_token *token, t_fsm *fsm);
int		is_assign_and_none(t_token *token, t_fsm *fsm);
int		is_token_redir_and_none(t_token *token, t_fsm *fsm);
int		tokenizer_tok_cmd_suffix(t_token *token, \
			t_fsm *fsm, t_input *input);
int		tokenizer_tok_cmd_name(t_token *token, \
			t_fsm *fsm, t_input *input);
int		tokenizer_do_tok_cmd(t_token *token, t_fsm *fsm, t_input *input);
int		run_tokenizer(t_token *token, t_fsm *fsm, t_input *input);

#endif
