/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/12 17:01:47 by arobu             #+#    #+#             */
/*   Updated: 2023/04/24 22:11:53 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "ft_printf.h"
#include "quote_list.h"
#include "fsm.h"

t_ast_node	*new_node(t_data data, t_node_type type, size_t sub_count)
{
	t_ast_node	*new_node;

	new_node = (t_ast_node *)malloc(sizeof(t_ast_node));
	if (!new_node)
		return (NULL);
	new_node->type = type;
	new_node->parent = NULL;
	new_node->right = NULL;
	new_node->left = NULL;
	new_node->is_subshell = sub_count;
	new_node->data = data;
	return (new_node);
}

void	ast_add_parent(t_ast_node **current, t_ast_node *new_parent)
{
	t_ast_node	*curr_node;

	if (!*current || !new_parent)
		return ;
	if (!*current)
		*current = new_parent;
	else
	{
		curr_node = *current;
		curr_node->parent = new_parent;
		if (new_parent->left == NULL)
			new_parent->left = curr_node;
		else
			new_parent->right = curr_node;
	}
	*current = (*current)->parent;
}

void	ast_add_left(t_ast_node *root, t_ast_node *left_child)
{
	t_ast_node	*curr_node;

	if (!root || !left_child)
		return ;
	else
	{
		curr_node = root;
		curr_node->left = left_child;
	}
}

void	ast_add_right(t_ast_node *root, t_ast_node *right_child)
{
	t_ast_node	*curr_node;

	if (!root || !right_child)
		return ;
	else
	{
		curr_node = root;
		curr_node->right = right_child;
		right_child->parent = curr_node;
	}
}

void	ast_add(t_ast_node **root, t_ast_node *add_node)
{
	if (!*root)
		*root = add_node;
	else if (is_single_node(*root))
		ast_add_parent(root, add_node);
	else if (has_left_child(*root) && \
				!has_parent(*root) && \
					!has_right_child(*root))
	{
		ast_add_right(*root, add_node);
	}
	else if (has_left_child(*root) && has_right_child(*root))
		ast_add_parent(root, add_node);
	return ;
}
