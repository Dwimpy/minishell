/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/12 17:01:50 by arobu             #+#    #+#             */
/*   Updated: 2023/04/25 17:33:25 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AST_H
# define AST_H

# include <stdlib.h>
# include "arglist.h"

typedef struct s_ast_node	t_ast_node;

typedef enum e_printing_branch
{
	LEFT,
	RIGHT
}				t_printing_branch;

typedef enum e_node_type
{
	COMMAND,
	PIPELINE,
	AND_IF,
	OR_IF,
	SUBSHELL
}				t_node_type;

typedef struct s_io_redirect
{
	int			fd_redir_out;
	int			is_appended;
	char		*filename;
}				t_io_redirect;

typedef struct s_cmd_prefix
{
	int					fd_redir_out;
	t_io_redirect		input;
	t_io_redirect		output;
	t_arglist			*assignments;
}				t_cmd_prefix;

typedef struct s_cmd_suffix
{
	int				fd_redir_out;
	t_io_redirect	input;
	t_io_redirect	output;
	t_arglist		*arglist;
}				t_cmd_suffix;

typedef struct s_command_info
{
	char			*name;
	t_arglist		*arglist;
	t_cmd_prefix	prefix;
	t_cmd_suffix	suffix;
	t_node_type		type;
}				t_command_info;

typedef struct s_cmd
{
	char		*name_path;
	char		**args;
	t_arglist	*assignments;
}				t_cmd;

typedef struct s_command
{
	t_cmd			cmd;
	t_io_redirect	input;
	t_io_redirect	output;
}				t_command;

typedef struct s_subshell
{
	t_ast_node	*root;
}				t_subshell;

typedef struct s_pipeline
{
	char		symbol;
}				t_pipeline;

typedef struct s_and_if
{
	char		*symbol;
}				t_and_if;

typedef struct s_and_or
{
	char		*symbol;
}				t_and_or;

typedef union u_data
{
	t_command		command;
	t_pipeline		pipeline;
	t_and_if		and_if;
	t_and_or		or_if;
	t_subshell		subshell;
}				t_data;

typedef struct s_ast_node
{
	int				is_subshell;
	t_node_type		type;
	t_data			data;
	t_ast_node		*parent;
	t_ast_node		*left;
	t_ast_node		*right;
}				t_ast_node;

t_ast_node	*new_node(t_data data, t_node_type type, size_t sub_count);
void		ast_add_parent(t_ast_node **current, t_ast_node *new_parent);
void		ast_add_left(t_ast_node *root, t_ast_node *left_child);
void		ast_add_right(t_ast_node *root, t_ast_node *right_child);
void		ast_add(t_ast_node	**root, t_ast_node *add_node);
int			is_single_node(t_ast_node *root);
int			has_right_child(t_ast_node *root);
int			has_left_child(t_ast_node *root);
int			has_parent(t_ast_node *root);
void		ast_set_type(t_ast_node *node, t_node_type type);
void		ast_del_node(t_ast_node	*node);
t_arglist	*expand_vars(char *value);

#endif
