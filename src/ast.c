/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/12 17:01:47 by arobu             #+#    #+#             */
/*   Updated: 2023/04/20 21:31:08 by arobu            ###   ########.fr       */
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
		// printf("%s\n", args[i]);
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

t_arglist	*expand_vars(char	*value)
{
	t_quotelist	*quotelist;
	t_quote		*quote;
	t_arglist	*arglist;
	t_fsm		fsm;
	int			i;
	int			start;

	if (!value)
		return (NULL);
	quotelist = create_list(value);
	arglist = new_arglist();
	quote = quotelist->first;
	fsm.input_state = N_INPUT;
	fsm.expand_var = NOT_IN_EXPAND_VAR;
	while (quote)
	{
		fsm.input_state = N_INPUT;
		i = 0;
		start = 0;
		while (fsm.input_state != INPUT_COMPLETE)
		{
			if (quote->value[i] == '\0')
			{
				if (start < i && quote->value[i - 1] != '\"' && quote->value[i - 1] != '$')
					new_argument(arglist, create_expand_arg(&quote->value[start], 0, i - start + 1, NON_EXPAND));
				fsm.input_state = INPUT_COMPLETE;
			}
			if (fsm.input_state == N_INPUT)
			{
				start = i + 1;
				if (quote->value[i] == '\'')
				{
					fsm.input_state = IN_SQUOTE;
					i++;
				}
				else if (quote->value[i] == '"')
				{
					fsm.input_state = IN_DQUOTE;
					i++;
				}
				else
				{
					fsm.input_state = REGULAR;
					start = i;
				}

			}
			if (quote->value[i] == '\\' && fsm.input_state != IN_SQUOTE && fsm.input_state != IN_DQUOTE)
			{
				if (start < i)
					new_argument(arglist, create_expand_arg(&quote->value[start], 0, i - start, NON_EXPAND));
				new_argument(arglist, create_expand_arg(&quote->value[i + 1], 0, 1, ESCAPED));
				i += 1;
				start = i + 1;
			}
			else if (fsm.expand_var == IN_EXPAND_VAR)
			{
				if (quote->value[i] == '$')
				{
					new_argument(arglist, create_expand_arg(&quote->value[start], 0, i - start, EXPAND));
					if (fsm.input_state == REGULAR)
						arglist->last->expand_type = 0;
					else if (fsm.input_state == IN_DQUOTE)
						arglist->last->expand_type = 1;
					fsm.expand_var = NOT_IN_EXPAND_VAR;
					start = i;
					i--;
				}
				else if (fsm.input_state == REGULAR && quote->value[i] == '\'')
				{
					fsm.expand_var = NOT_IN_EXPAND_VAR;
					fsm.input_state = IN_SQUOTE;
					start = i + 1;
				}
				else if (!ft_isalnum(quote->value[i]) && quote->value[i] != '?' && quote->value[i] != '_')
				{
					new_argument(arglist, create_expand_arg(&quote->value[start], 0, i - start, EXPAND));
					if (fsm.input_state == REGULAR)
						arglist->last->expand_type = 0;
					else if (fsm.input_state == IN_DQUOTE)
						arglist->last->expand_type = 1;
					fsm.expand_var = NOT_IN_EXPAND_VAR;
					start = i;
				}
				else if (quote->value[i + 1] == '\0')
				{
					new_argument(arglist, create_expand_arg(&quote->value[start], 0, i - start + 1, EXPAND));
					if (fsm.input_state == REGULAR)
						arglist->last->expand_type = 0;
					else if (fsm.input_state == IN_DQUOTE)
						arglist->last->expand_type = 1;
					fsm.expand_var = NOT_IN_EXPAND_VAR;
					start = i + 1;
				}
			}
			else if (fsm.input_state == IN_SQUOTE)
			{
				if (quote->value[i] == '\'')
				{
					new_argument(arglist, create_expand_arg(&quote->value[start], 0, i - start, NON_EXPAND));
					fsm.input_state = REGULAR;
					start = i + 1;
				}
			}
			else if (fsm.input_state == IN_DQUOTE)
			{
				if (quote->value[i] == '$')
				{
					if (start < i)
						new_argument(arglist, create_expand_arg(&quote->value[start], 0, i - start, NON_EXPAND));
					fsm.expand_var = IN_EXPAND_VAR;
					start = i;
				}
				else if (quote->value[i] == '\"')
				{
					new_argument(arglist, create_expand_arg(&quote->value[start], 0, i - start, NON_EXPAND));
					fsm.input_state = REGULAR;
					start = i + 1;
				}
				else if (!ft_isalnum(quote->value[i]))
				{
					new_argument(arglist, create_expand_arg(&quote->value[start], 0, i - start + 1, NON_EXPAND));
					start = i + 1;
				}
			}
			else if (fsm.input_state == REGULAR)
			{
				if (quote->value[i] == '$')
				{
					if (start < i)
						new_argument(arglist, create_expand_arg(&quote->value[start], 0, i - start, NON_EXPAND));
					fsm.expand_var = IN_EXPAND_VAR;
					start = i;
				}
				else if (quote->value[i] == '\'')
					fsm.input_state = IN_SQUOTE;
				else if (quote->value[i] == '\"')
					fsm.input_state = IN_DQUOTE;
				else if (!ft_isalnum(quote->value[i]))
				{
					new_argument(arglist, create_expand_arg(&quote->value[start], 0, i - start + 1, NON_EXPAND));
					start = i + 1;
				}
				else if (quote->value[i + 1] == '\0')
				{
					new_argument(arglist, create_expand_arg(&quote->value[start], 0, i - start + 1, NON_EXPAND));
					start = i + 1;
				}
			}		
			i++;
		}
		quote = quote->next;
	}
	// print_quotelist(quotelist);
	// print_args(arglist);
	free_quotelist(quotelist);
	return (arglist);
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

void	print_node(t_ast_node *node)
{
	if (node->type == COMMAND && node->data.command.cmd.name_path)
		printf("%s\n", node->data.command.cmd.name_path);
	else if (node->type == COMMAND && node->data.command.cmd.assignments)
		printf("%s\n", node->data.command.cmd.assignments->first->value);
	else if (node->type == PIPELINE)
		printf("%c\n", node->data.pipeline.symbol);
	else if (node->type == AND_IF)
		printf("%s\n", node->data.and_if.symbol);
	else if (node->type == OR_IF)
		printf("%s\n", node->data.or_if.symbol);
}

void	print_syntax_error(int unexpected)
{
	if (unexpected != TOKEN_END_OF_FILE)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
		if (unexpected == TOKEN_EOF)
			ft_putstr_fd("newline'\n", 2);
		else if (unexpected == TOKEN_PIPE)
			ft_putstr_fd("|'\n", 2);
		else if (unexpected == TOKEN_LESS)
			ft_putstr_fd("<'\n", 2);
		else if (unexpected == TOKEN_GREAT)
			ft_putstr_fd(">'\n", 2);
		else if (unexpected == TOKEN_DLESS)
			ft_putstr_fd("<<'\n", 2);
		else if (unexpected == TOKEN_DGREAT)
			ft_putstr_fd(">>'\n", 2);
		else if (unexpected == TOKEN_AND)
			ft_putstr_fd("&'\n", 2);
		else if (unexpected == TOKEN_AND_IF)
			ft_putstr_fd("&&'\n", 2);
		else if (unexpected == TOKEN_OR_IF)
			ft_putstr_fd("||'\n", 2);
		else if (unexpected == TOKEN_RPARENTHESIS)
			ft_putstr_fd(")'\n", 2);
		else if (unexpected == TOKEN_LPARENTHESIS)
			ft_putstr_fd("('\n", 2);	
	}
	else
		ft_putstr_fd("minishell: syntax error: unexpected end of file\n", 2);
}
