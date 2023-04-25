/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   x_execution.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkilling <tkilling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 15:11:41 by tkilling          #+#    #+#             */
/*   Updated: 2023/04/24 14:14:20 by tkilling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "x_execution.h"

void	ft_change_tree(t_ast_node *root)
{
	t_ast_node	*ptr;

	ptr = root->parent;
	while (ptr)
	{
		ptr->right->left = ptr->right->parent;
		ptr->right->parent = ptr->parent;
		ptr = ptr->parent;
	}
}

void	ft_change_tree_back(t_ast_node *root)
{
	t_ast_node	*ptr;

	ptr = root->parent;
	while (ptr)
	{
		ptr->right->left = NULL;
		ptr = ptr->parent;
	}
}

int	ft_execution(t_input *input, t_ast_node *root, int *fd)
{
	int			exit_code;
	pid_t		pid;
	int			status;

	exit_code = 0;
	while (root->left != NULL)
		root = root->left;
	ft_change_tree(root);
	input->sub = 0;
	exit_code = ft_execute_tree(input, root, fd);
	ft_change_tree_back(root);
	pid = 1;
	while (pid != -1)
		pid = waitpid(-1, &status, 0);
	while (*fd > 2)
		close((*fd)--);
	return (exit_code);
}
