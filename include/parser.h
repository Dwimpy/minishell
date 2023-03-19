/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/12 13:28:56 by arobu             #+#    #+#             */
/*   Updated: 2023/03/19 13:12:49 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "lexer.h"
# include "ast.h"

int			accept(t_token *token, t_token_type type);
int			expect(t_token *token, t_token_type type);
t_ast_node	*parse_command(t_token_list *list);
int			parse_prefix(t_token_list *list);
int			parse_io_file(t_token_list *list);

#endif
