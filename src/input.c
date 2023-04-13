/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/03 00:42:57 by arobu             #+#    #+#             */
/*   Updated: 2023/04/13 04:08:55 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "environment_handler.h"
#include "quote_list.h"

static int	is_valid_string(t_input *input);

void	parse_all_input(t_input *input)
{
	t_token	*token;

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
	print_tree(input->root);
	// printf("PRINT: %s\n", input->root->left->data.and_if.symbol);
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
