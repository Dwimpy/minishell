/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/12 17:01:50 by arobu             #+#    #+#             */
/*   Updated: 2023/03/22 21:23:35 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AST_H
# define AST_H

# include <stdlib.h>
# include "arglist.h"

typedef struct s_ast_node	t_ast_node;

typedef enum e_node_type
{
	COMMAND	,
	PIPELINE
}				t_node_type;

typedef struct s_io_redirect
{
	char		*filename;
}				t_io_redirect;

typedef struct s_assignment_word
{
	t_arglist	*arglist;
}				t_assignment_word;

typedef struct s_cmd_prefix
{
	t_io_redirect		input;
	t_io_redirect		output;
	t_assignment_word	assignments;
}				t_cmd_prefix;

typedef struct s_cmd_suffix
{
	t_io_redirect	input;
	t_io_redirect	output;
	t_arglist		*arglist;
}				t_cmd_suffix;

typedef struct s_command
{
	char			*name;
	t_arglist		*arglist;
	t_cmd_prefix	prefix;
	t_cmd_suffix	suffix;
	t_node_type		type;
}				t_command;

typedef struct s_pipeline
{
	t_node_type		type;
}				t_pipeline;

typedef union u_data
{
	t_command		command;
	t_pipeline		pipeline;
}				t_data;

typedef struct s_ast_node
{
	t_node_type		type;
	t_data			data;
	t_ast_node		*parent;
	t_ast_node		*left;
	t_ast_node		*right;
}				t_ast_node;

t_ast_node	*new_node(t_data data);
t_arglist	*new_arg(t_arglist *prev, char *value);
void		ast_add_parent(t_ast_node **current, t_ast_node *new_parent);
void		ast_add_left(t_ast_node *root, t_ast_node *left_child);
void		ast_add_right(t_ast_node *root, t_ast_node *right_child);
void		ast_add(t_ast_node	**root);
void		ast_set_type(t_ast_node *node, t_node_type type);
void		ast_del_node(t_ast_node	*node);

#endif
