/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/12 17:01:50 by arobu             #+#    #+#             */
/*   Updated: 2023/03/18 19:34:54 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AST_H
# define AST_H

# include <stdlib.h>

typedef struct s_ast_node	t_ast_node;

typedef enum e_node_type
{
	SIMPLE_COMMAND,
	PIPELINE,
}				t_node_type;

typedef struct s_simple_cmd
{
	char		*name;
	char		**args;
}				t_simple_cmd;

typedef struct s_pipeline
{

}				t_pipeline;

typedef union u_data
{
	t_simple_cmd	simple_cmd;
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
void		ast_add_parent(t_ast_node **current, t_ast_node *new_parent, \
					t_node_type type);
void		ast_add_left(t_ast_node *root, t_ast_node *left_child, \
					t_node_type type);
void		ast_add_right(t_ast_node *root, t_ast_node *right_child, \
					t_node_type type);

#endif
