/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/05 22:15:18 by dwimpy            #+#    #+#             */
/*   Updated: 2023/03/21 15:09:01 by arobu            ###   ########.fr       */
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
	t_ast_node		*ast_node;
	t_token			*token;

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
	while (1)
	{
		init_lexer(&lexer);
		get_tokens(tokens, lexer);
		print_tokens(tokens);
		printf("Size: %zu\n", tokens->num_tokens);
		ast_node = parse_command(tokens);
		if (ast_node->data.command.prefix.assignments.arglist)
			printf("Assignment: %s\n", ast_node->data.command.prefix.assignments.arglist->first->value);
		if (ast_node)
		{
			if (ast_node->data.command.name != NULL)
			{
				printf("Command: %s\n", ast_node->data.command.name);
				printf("Input Redir: %s\n", ast_node->data.command.prefix.input.filename);
				printf("Output Redir: %s\n", ast_node->data.command.prefix.output.filename);
				print_args(ast_node->data.command.arglist);
			}
		}
		if (ast_node->data.command.name && ft_strncmp(ast_node->data.command.name, "exit", 5) == 0)
		{
			free_token_list(tokens);
			free(lexer.input);
			system("leaks minishell");
			exit(0);
		}
		ast_del_node(ast_node);
		free(lexer.input);
		free_token_list(tokens);
	}
	return (0);
}
