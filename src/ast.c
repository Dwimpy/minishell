/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/12 17:01:47 by arobu             #+#    #+#             */
/*   Updated: 2023/03/25 18:07:31 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "ft_printf.h"

t_ast_node	*new_node(t_data data, t_node_type type)
{
	t_ast_node	*new_node;

	new_node = (t_ast_node *)malloc(sizeof(t_ast_node));
	if (!new_node)
		return (NULL);
	new_node->type = type;
	new_node->parent = NULL;
	new_node->right = NULL;
	new_node->left = NULL;
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
	if (!root)
		root = left_child;
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
	if (!root)
		root = right_child;
	else
	{
		curr_node = root;
		curr_node->right = right_child;
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
		ast_add_right(*root, add_node);
	else if (has_left_child(*root) && has_right_child(*root))
		ast_add_parent(root, add_node);
	return ;
}

int	is_single_node(t_ast_node *root)
{
	return (!root->parent && !root->left && !root->right);
}

int	has_right_child(t_ast_node *root)
{
	return(root->right != NULL);
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

void	ast_del_node(t_ast_node *node)
{
	if (!node)
		return ;
	ast_del_node((node)->left);
	ast_del_node((node)->right);
	// free_args(node->data.command.arglist);
	// if (node->data.command.name)
	// 	free(node->data.command.name);
	// if (node->data.command.prefix.input.filename)
	// 	free(node->data.command.prefix.input.filename);
	// if (node->data.command.prefix.output.filename)
	// 	free(node->data.command.prefix.output.filename);
	// free_args(node->data.command.prefix.assignments);
	if (node)
		free(node);
	node = NULL;
}

void	print_tree_helper(t_ast_node *node, int level)
{
	if (node == NULL) {
		return;
	}

    // Print the right subtree
    print_tree_helper(node->right, level + 1);

    // Print the current node
    for (int i = 0; i < level; i++) {
        printf("    ");
    }

    if (node->type == COMMAND) 
        printf("%s\n", node->data.command.name);
    else if (node->type == PIPELINE)
        printf("|\n");
	else if (node->type == AND_IF)
		printf("%s\n", node->data.and_if.symbol);
	else if (node->type == OR_IF)
		printf("%s\n", node->data.or_if.symbol);
	else
		printf("(null)");
    // Print the left subtree
    print_tree_helper(node->left, level + 1);
}

void print_tree(t_ast_node *root)
{
    print_tree_helper(root, 0);
}
