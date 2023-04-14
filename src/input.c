/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/03 00:42:57 by arobu             #+#    #+#             */
/*   Updated: 2023/04/14 14:05:27 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "environment_handler.h"
#include "quote_list.h"

static int	is_valid_string(t_input *input);

void	parse_all_input(t_input *input)
{
	t_token	*token;
	char	*exp;
	token = input->tokens->first;
	while (token)
	{
		if (token->type == TOKEN_QUOTE)
			print_quotelist(token->value.quote.quotes);
		token = token->next;
	}
	while (input->tokens->first->type != TOKEN_EOF)
		parse_input(&input->root, input->tokens, input);
	if (input->root->type == COMMAND && input->root->data.command.cmd.name_path && ft_strncmp(input->root->data.command.cmd.name_path, "exit", 5) == 0)
	{
		ast_del_node(input->root);
		input->root = NULL;
		clear_history();
		free(input->lexer.input);
		free_token_list(input->tokens);
		free(input->tokens);
		system("leaks minishell");
		exit(0);
	}
	if (input->root->type == COMMAND && input->root->data.command.cmd.name_path)
	{
		exp = expand_env_var(input->root->data.command.cmd.name_path, input);
		printf("%s\n", exp);
		free(exp);
	}
	// print_tree(input->root);
	// printf("PRINT: %s\n", input->root->left->data.and_if.symbol);
}

void	expand_command_node(t_ast_node *root, t_input *input)
{
	if (root->data.command.cmd.name_path)
		root->data.command.cmd.name_path = expand_env_var(root->data.command.cmd.name_path, input);
}

char	*expand_env_var(char *value, t_input *input)
{
	t_arglist	*list;
	t_arg		*arg;
	size_t		new_len;
	char		*new_value;
	char		*entry;
	int			prev_index;

	if (value)
	{
		list = expand_vars(value);
		new_len = ft_strlen(value);
	}
	else
		return (ft_strdup(""));
	if (!arg)
	{
		free_args(list);
		return (NULL);
	}
	arg = list->first;
	while (arg)
	{
		new_len -= arg->len;
		entry = (char *)hashmap_get(input->hashmap, arg->value);
		if (entry)
			new_len += ft_strlen(entry);
		else
			new_len += 0;
		arg = arg->next;
	}
	new_value = (char *)ft_calloc(new_len + 1, sizeof(char));
	if (!new_value)
		return (NULL);
	arg = list->first;
	if (arg && arg->start_pos != 0)
		ft_strlcat(new_value, &value[0], arg->start_pos + 1);
	while (arg)
	{
		prev_index = arg->start_pos + arg->len;
		entry = (char *)hashmap_get(input->hashmap, arg->value);
		if (entry)
			ft_strcat(new_value, entry);
		else
			ft_strcat(new_value, "");
		arg = arg->next;
		if (arg)
			ft_strncat(new_value, &value[prev_index], arg->start_pos - prev_index);
	}
	if (prev_index < ft_strlen(value))
		ft_strncat(new_value, &value[prev_index], ft_strlen(value) - prev_index);
	free_args(list);
	return (new_value);
}

void	init_input(t_input	*input, char **envp)
{
	input->tokens = new_token_list();
	input->hashmap = load_environment(envp);
	input->root = NULL;
	input->lexer.ch = '\0';
	input->lexer.input = NULL;
	input->lexer.input_len = 0;
	input->lexer.read_position = -1;
	input->lexer.tok_position = -1;
	input->unexpected = 0;
	return ;
}

int	generate_input(t_input *input)
{
	t_token		*token;

	// if (init_lexer(input))
	// 	return (1);
	// while (is_valid_string(input))
	// {
	// 	free_token_list(input->tokens);
	// 	analyze_input(&input->lexer);
	// 	get_tokens(input->tokens, input->lexer);
	// 	token = get_last_token(input->tokens);
	// 	if (token && append_input_pipe(&input->lexer, token->type))
	// 		break ;
	// 	input->lexer.read_position = -1;
	// }
	// if (analyze_syntax(input->tokens, &input->unexpected) != 0)
	// {
	// 	ft_putstr_fd("incorrect syntax near", 2);
	// 	printf(" %d\n", input->unexpected);
	// 	free(input->lexer.input);
	// 	free_token_list(input->tokens);
	// 	return (2);
	// }
	print_tokens(input->tokens);
	return (0);
}

static int	is_valid_string(t_input *input)
{
	return (input->tokens->num_tokens == 0 || \
		(get_last_token(input->tokens)->type != TOKEN_WORD) && \
		get_last_token(input->tokens)->type != TOKEN_SEMICOLON);
}
