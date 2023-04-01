/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/12 13:28:56 by arobu             #+#    #+#             */
/*   Updated: 2023/04/01 19:09:22 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include <unistd.h>
# include <fcntl.h>
# include "lexer.h"
# include "ast.h"
# include "syntax_analyzer.h"

# define INPUT 0
# define OUTPUT 1

int				accept(t_token *token, t_token_type type);
int				expect(t_token *token, t_token_type type);
int				accept_redirection(t_token *token);
t_ast_node		*parse_command(t_token_list *list);
t_ast_node		*parse_pipeline(t_token_list *tokens);
t_ast_node		*parse_and_if(t_token_list *tokens);
t_ast_node		*parse_or_if(t_token_list *tokens);
t_ast_node		*parse_subshell(t_token_list *tokens);
void			parse_input(t_ast_node **root, t_token_list *list);
void			parse_redirection_prefix(t_token_list *tokens, \
					t_cmd_prefix *prefix);
void			parse_redirection_suffix(t_token_list *tokens, \
					t_cmd_suffix *suffix);
void			parse_suffix_words(t_token_list *tokens, t_cmd_suffix *suffix);
void			parse_assignment(t_token_list *tokens, t_cmd_prefix *prefix);
void			parse_cmd_word(t_token_list	**tokens, t_data *data);
t_cmd_prefix	parse_prefix(t_token_list **tokens);
t_cmd_suffix	parse_suffix(t_token_list **tokens);
int				is_prefix(t_token *token);
int				is_input_redir(t_token *token);
int				is_output_redir(t_token *token);
int				is_assign_word(t_token *token);
int				is_cmd_word(t_token *token);
int				is_cmd_suffix(t_token *token);
void			create_and_free(t_token *token, char **filename, int io);
int				is_pipe(t_token *token);
int				is_logical_op(t_token *token);
int				is_prev_subshell(t_token *token);
#endif
