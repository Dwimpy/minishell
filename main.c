/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/05 22:15:18 by dwimpy            #+#    #+#             */
/*   Updated: 2023/04/01 19:35:45 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "environment_handler.h"

int	main(int argc, char **argv, char **envp)
{
	t_token_list	*tokens;
	t_lexer			lexer;
	t_hashmap		*hashmap;
	t_ast_node		*root;
	t_ast_node		*ast_node;
	t_token			*token;
	int				unexpected;
	int				history_elements;

	tokens = new_token_list();
	array = (char **)hashmap_toarray(hashmap, NULL);

	root = NULL;
	unexpected = 0;
	hashmap = load_environment(envp);
	printf("%s\n", hashmap_get(hashmap, "HOMEBREW_CACHE"));
	while (1)
	{
		if (init_lexer(&lexer))
			continue ;
		analyze_input(&lexer);
		get_tokens(tokens, lexer);
		print_tokens(tokens);
		if (analyze_syntax(tokens, &unexpected) != 0)
		{
			ft_putstr_fd("incorrect syntax near", 2);
			printf(" %d\n", unexpected);
			free(lexer.input);
			free_token_list(tokens);
			continue ;
		}
		while (tokens->first->type != TOKEN_EOF)
			parse_input(&root, tokens);
		if (root && root->type == COMMAND && \
			ft_strncmp(root->data.command.name, "exit", 5) == 0)
		{
			free(lexer.input);
			clear_history();
			free_token_list(tokens);
			ast_del_node(root);
			hashmap_free(&hashmap);
			system("leaks minishell");
			exit(0);
		}
		// printf("Size: %zu\n", tokens->num_tokens);
		// printf("\t\t%s\n", root->data.command.name);
		// printf("\t%c\t\t%s\n", '|', "(null)");
		// printf("%s\t", root->left->left->data.command.name);
		// printf("\t%s\n", root->left->right->data.command.name);
		print_tree(root);
		// printf("%s\t\t\n", root->data.command.name);
		// if (ast_node->data.command.prefix.assignments)
		// 	printf("Assignment: %s\n", ast_node->data.command.prefix.assignments->first->value);
		// if (ast_node)
		// {
		// 	if (ast_node->data.command.name != NULL)
		// 	{
		// 		printf("Command: %s\n", ast_node->data.command.name);
		// 		printf("P: Input Redir: %s\n", ast_node->data.command.prefix.input.filename);
		// 		printf("P: Output Redir: %s\n", ast_node->data.command.prefix.output.filename);
		// 		printf("S: Input Redir: %s\n", ast_node->data.command.suffix.input.filename);
		// 		printf("S: Output Redir: %s\n", ast_node->data.command.suffix.output.filename);
		// 		print_args(ast_node->data.command.arglist);
		// 		print_args(ast_node->data.command.suffix.arglist);
		// 	}
		// }
		// if (ast_node->data.command.name && ft_strncmp(ast_node->data.command.name, "exit", 5) == 0)
		// {
		// 	free_token_list(tokens);
		// 	free(lexer.input);
		// 	system("leaks minishell");
		// 	exit(0);
		// }
		ast_del_node(root);
		root = NULL;
		free(lexer.input);
		free_token_list(tokens);
	}
	return (0);
}
