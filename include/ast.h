/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/12 17:01:50 by arobu             #+#    #+#             */
/*   Updated: 2023/03/19 14:17:40 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AST_H
# define AST_H

# include <stdlib.h>
# include "arglist.h"

typedef struct s_ast_node	t_ast_node;

typedef enum e_node_type
{
	SIMPLE_COMMAND,
	PIPELINE,
}				t_node_type;

typedef struct s_redirection
{
	int			fd;
	char		*file;
}				t_redirection;

typedef struct s_command
{
	char			*name;
	t_arglist		*arglist;
	t_node_type		type;
	t_redirection	input;
	t_redirection	output;
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
void		ast_set_type(t_ast_node *node, t_node_type type);
void		ast_del_node(t_ast_node	*node);

#endif
