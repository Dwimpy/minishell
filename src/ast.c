/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/12 17:01:47 by arobu             #+#    #+#             */
/*   Updated: 2023/04/10 23:49:00 by arobu            ###   ########.fr       */
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

void	free_cmd_args(char **args)
{
	int	i;

	i = 0;
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
}

void	ast_del_node(t_ast_node *node)
{
	if (!node)
		return ;
	ast_del_node((node)->left);
	if (node->type == COMMAND)
	{
		if (node->data.command.cmd.name_path)
			free(node->data.command.cmd.name_path);
		if (node->data.command.cmd.assignments)
			free_args(node->data.command.cmd.assignments);
		if (node->data.command.input.filename)
			free(node->data.command.input.filename);
		if (node->data.command.output.filename)
			free(node->data.command.output.filename);
		if (node->data.command.cmd.args)
			free_cmd_args(node->data.command.cmd.args);
	}
	if (node->type == AND_IF)
		free(node->data.and_if.symbol);
	if (node->type == OR_IF)
		free(node->data.and_if.symbol);
	ast_del_node((node)->right);
	if (node)
		free(node);
	node = NULL;
}

t_ast_node *from_identidier_to_tree(t_ast_node *node, t_printing_branch branch)
{
	if (branch == LEFT)
		return (node->left);
	else
		return (node->right);
}

void	print_identifier(t_printing_branch branch)
{
	if (branch == LEFT)
		printf("L");
	else
		printf("R");
}

void	print_tree_helper(t_ast_node *node, int level, t_printing_branch branch)
{
	if (node == NULL) {
		return;
	}
    // Print the right subtree
    print_tree_helper(from_identidier_to_tree(node, branch), level + 1, branch);
    // Print the current node
    for (int i = 0; i < level; i++) {
        printf("    .");
    }
	// print_identifier(order, branch);
    if (node->type == COMMAND && node->data.command.cmd.name_path)
        printf("%s\n", node->data.command.cmd.name_path);
	else if (node->type == COMMAND && !node->data.command.cmd.name_path && node->data.command.cmd.assignments)
        printf("A: %s!\n", node->data.command.cmd.assignments->first->value);
    else if (node->type == PIPELINE)
        printf("|\n");
	else if (node->type == AND_IF)
		printf("%s\n", node->data.and_if.symbol);
	else if (node->type == OR_IF)
		printf("%s\n", node->data.or_if.symbol);
	else if (node->type == SUBSHELL)
		printf("SUBSHELL\n");
    // Print the left subtree
    print_tree_helper(from_identidier_to_tree(node, !branch), level + ((node->type != COMMAND)), branch);
}

void print_tree(t_ast_node *root)
{
    print_tree_helper(root, 0, LEFT);
	printf("--------\n");
}
