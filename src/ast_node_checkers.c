/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_node_checkers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 21:12:41 by arobu             #+#    #+#             */
/*   Updated: 2023/04/24 21:13:09 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "ft_printf.h"
#include "quote_list.h"
#include "fsm.h"

int	is_single_node(t_ast_node *root)
{
	return (!root->parent && !root->left && !root->right);
}

int	has_right_child(t_ast_node *root)
{
	return (root->right != NULL);
}

int	has_left_child(t_ast_node *root)
{
	return (root->left != NULL);
}

int	has_parent(t_ast_node *root)
{
	return (root->parent != NULL);
}

void	ast_set_type(t_ast_node *node, t_node_type type)
{
	node->type = type;
}
