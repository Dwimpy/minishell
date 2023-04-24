/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   x_execute_tree.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkilling <tkilling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 11:30:30 by tkilling          #+#    #+#             */
/*   Updated: 2023/04/24 12:53:45 by tkilling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "x_execution.h"

void	ft_no_cmd(t_input *input, t_ast_node **root, int *fd, int *exit_code);
void	ft_is_cmd(t_input *input, t_ast_node **root, int *fd, int *exit_code);
int		ft_no_tree(t_input *input, t_ast_node *root);
void	ft_need_sub(t_input *input, t_ast_node **root, int *fd, int *exit_code);

int	ft_execute_tree(t_input *input, t_ast_node *root, int *fd)
{
	int		exit_code;

	exit_code = 0;
	while (root)
	{
		if (root->type == COMMAND)
		{
			if (root->parent == NULL && root->left == NULL)
				return (ft_no_tree(input, root));
			else if (root->is_subshell != input->sub)
				ft_need_sub(input, &root, fd, &exit_code);
			else
				ft_is_cmd(input, &root, fd, &exit_code);
		}
		else
		{
			if (root->is_subshell < input->sub)
				exit(exit_code);
			ft_no_cmd(input, &root, fd, &exit_code);
		}
		root = root->parent;
	}
	if (input->sub)
		exit(exit_code);
	return (exit_code);
}

void	ft_need_sub(t_input *input, t_ast_node **root, int *fd, int *exit_code)
{
	*exit_code = ft_add_subshell(input, (*root), fd, input->sub + 1);
	while ((*root)->parent && (*root)->parent->is_subshell > input->sub)
		(*root) = (*root)->parent;
}

int	ft_no_tree(t_input *input, t_ast_node *root)
{
	if (root->is_subshell == 0)
		return (ft_command(root->data.command.cmd.args, input, root));
	else
		return (ft_subshell_no_tree(input, root));
}

void	ft_is_cmd(t_input *input, t_ast_node **root, int *fd, int *exit_code)
{
	if ((*root)->is_subshell == input->sub && (*root)->left == NULL)
	{
		if ((*root)->parent->type == PIPELINE)
			*exit_code = ft_pipe(input, (*root), fd);
		else
			*exit_code = ft_and_if(input, (*root), fd);
	}
	else if ((*root)->is_subshell == input->sub)
	{
		if ((*root)->left->type == PIPELINE)
			*exit_code = ft_pipe(input, (*root), fd);
		else if ((*root)->left->type == AND_IF && *exit_code == 0)
			*exit_code = ft_and_if(input, (*root), fd);
		else if ((*root)->left->type == OR_IF && *exit_code != 0)
			*exit_code = ft_or_if(input, (*root), fd);
		else
		{
			while ((*root)->parent && (*root)->parent->type == PIPELINE)
				(*root) = (*root)->parent;
		}
	}
}

void	ft_no_cmd(t_input *input, t_ast_node **root, int *fd, int *exit_code)
{
	if ((*root)->right->is_subshell != input->sub)
	{
		if ((*exit_code == 0 && (*root)->type == AND_IF)
			|| (*exit_code != 0 && (*root)->type == OR_IF)
			|| (*root)->type == PIPELINE)
			*exit_code = ft_add_subshell(input, (*root)->right,
					fd, input->sub + 1);
		while ((*root)->parent && (*root)->parent->is_subshell > input->sub)
			(*root) = (*root)->parent;
	}
	else
	{
		if ((*root)->right->is_subshell == input->sub)
		{
			if ((*root)->type == PIPELINE)
				*exit_code = ft_pipe(input, (*root)->right, fd);
			else if ((*root)->type == AND_IF && *exit_code == 0)
				*exit_code = ft_and_if(input, (*root)->right, fd);
			else if ((*root)->type == OR_IF && *exit_code != 0)
				*exit_code = ft_or_if(input, (*root)->right, fd);
			else
				while ((*root)->parent && (*root)->parent->type == PIPELINE)
					(*root) = (*root)->parent;
		}
	}
}
