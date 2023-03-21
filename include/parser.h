/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/12 13:28:56 by arobu             #+#    #+#             */
/*   Updated: 2023/03/21 14:57:08 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "lexer.h"
# include "ast.h"

int				accept(t_token *token, t_token_type type);
int				expect(t_token *token, t_token_type type);
int				accept_redirection(t_token *token);
t_ast_node		*parse_command(t_token_list *list);
void			parse_redirection(t_token_list *tokens, t_cmd_prefix *prefix);
void			parse_assignment(t_token_list *tokens, t_cmd_prefix *prefix);
int				is_prefix(t_token *token);
int				is_input_redir(t_token *token);
int				is_output_redir(t_token *token);
int				is_assign_word(t_token *token);
t_cmd_prefix	parse_prefix(t_token_list **tokens);
int				parse_io_file(t_token *token);
#endif
