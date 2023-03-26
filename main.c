/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/05 22:15:18 by dwimpy            #+#    #+#             */
/*   Updated: 2023/03/26 19:00:19 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "hashmap.h"

int	main(int argc, char **argv, char **envp)
{
	t_token_list	*tokens;
	t_lexer			lexer;
	t_hashmap		*hashmap;
	t_ast_node		*root;
	t_ast_node		*ast_node;
	t_token			*token;
	int				unexpected;

	tokens = new_token_list();
	// add_token(tokens, new_token(TOKEN_WORD, "FUCK OFF"));
	// add_token(tokens, new_token(TOKEN_WORD, "FUCK OF"));
	// add_token(tokens, new_token(TOKEN_WORD, "FUCK O"));
	// parse_prefix(&tokens);
	// printf("%s", tokens->first->value.word.value);
	// hashmap = hashmap_new(100);
	// // printf("%02x\n", table_hash(tokens));
	// hashmap_put(hashmap, "bro", "what");
	// hashmap_put(hashmap, "key", "WTF");
	// hashmap_put(hashmap, "asd", "hello");
	// hashmap_put(hashmap, "fds", "Grav");
	// hashmap_put(hashmap, "fdds", "Grav");
	// array = (char **)hashmap_toarray(hashmap, NULL);
	// hashmap_remove(hashmap, "key");
	// hashmap_remove(hashmap, "fdds");
	// hashmap_remove(hashmap, "fds");
	// hashmap_remove(hashmap, "asd");
	// hashmap_remove(hashmap, "bro");
	// printf("\n%s\n", array[4]);
	// free(array);
	// if ((char *)hashmap_get(hashmap, "bro") == NULL)
	// 	printf("NULL");
	// hashmap_free(&hashmap);
	// printf("%d", hashmap_length(hashmap));
	// printf("%s", envp[0]);
	root = NULL;
	while (1)
	{
		init_lexer(&lexer);
		get_tokens(tokens, lexer);
		print_tokens(tokens);
		// if (analyze_syntax(tokens, &unexpected) != 0)
		// {
		// 	ft_putstr_fd("incorrect syntax near", 2);
		// 	printf(" %d\n", unexpected);
		// 	free(lexer.input);
		// 	free_token_list(tokens);
		// 	continue ;
		// }
		while (tokens->first->type != TOKEN_EOF)
		{
			ast_add(&root, parse_command(tokens));
			ast_add(&root, parse_pipeline(tokens));
			ast_add(&root, parse_and_if(tokens));
			ast_add(&root, parse_or_if(tokens));
			ast_add(&root, parse_subshell(tokens));
		}
		printf("Size: %zu\n", tokens->num_tokens);
		// t_ast_node	*test;
		// t_ast_node	*test2;
		// t_ast_node	*test3;
		// t_ast_node	*test4;
		// t_ast_node	*test5;
		// t_ast_node	*test6;
		// t_ast_node	*test7;
		// t_ast_node	*test8;
		// test = new_node((t_data){.pipeline.type = PIPELINE}, PIPELINE);
		// test2 = new_node((t_data){.command.name = ft_strdup("echo")}, COMMAND);
		// test3 = new_node((t_data){.pipeline.type = PIPELINE}, PIPELINE);
		// test4 = new_node((t_data){.command.name = ft_strdup("cat")}, COMMAND);
		// test5 = new_node((t_data){.pipeline.type = PIPELINE}, PIPELINE);
		// test6 = new_node((t_data){.command.name = ft_strdup("wtf")}, COMMAND);
		// test7 = new_node((t_data){.pipeline.type = PIPELINE}, PIPELINE);
		// test8 = new_node((t_data){.command.name = ft_strdup("wtf")}, COMMAND);
		// ast_add(&root, ast_node);
		// ast_add(&root, test);
		// ast_add(&root, test2);
		// ast_add(&root, test3);
		// ast_add(&root, test4);
		// ast_add(&root, test5);
		// ast_add(&root, test6);
		// ast_add(&root, test7);
		// ast_add(&root, test8);
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
		// ast_del_node(root);
		root = NULL;
		free(lexer.input);
		free_token_list(tokens);
	}
	return (0);
}
