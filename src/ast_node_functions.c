/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_node_functions.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 21:13:45 by arobu             #+#    #+#             */
/*   Updated: 2023/04/24 21:13:53 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "ft_printf.h"
#include "quote_list.h"
#include "fsm.h"

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
