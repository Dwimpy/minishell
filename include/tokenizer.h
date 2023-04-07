/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/06 19:26:01 by arobu             #+#    #+#             */
/*   Updated: 2023/04/07 18:35:12 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENIZER_H
# define TOKENIZER_H

# include "lexer.h"
# include "fsm.h"

int		gen_input(t_input *input);
void	tokenize(t_input *input, t_fsm *fsm);
void	get_the_input(t_input *input, t_fsm *fsm);
void	do_linebreak(t_lexer *lexer, char *prompt, t_fsm *fsm);
void	do_squote(t_lexer *lexer, t_fsm *fsm);
void	do_dquote(t_lexer *lexer, t_fsm *fsm);
void	do_subsh(t_lexer *lexer, t_fsm *fsm);
void	do_in_pipe(t_lexer *lexer, t_fsm *fsm);
void	do_in_cmdand(t_lexer *lexer, t_fsm *fsm);
void	do_in_cmdor(t_lexer *lexer, t_fsm *fsm);
void	readline_new_line(t_lexer *lexer, char *prompt, t_fsm *fsm);
void	readline_no_new_line(t_lexer *lexer, char *prompt, t_fsm *fsm);
void	readline_pipe(t_lexer *lexer, char *prompt, t_fsm *fsm);
char	*get_prompt_dir(void);
char	*read_from_stdin(void);
void	fsm_input_state_update(char c, t_lexer *lexer, t_fsm *fsm);
#endif
