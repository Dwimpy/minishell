/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/05 22:15:18 by dwimpy            #+#    #+#             */
/*   Updated: 2023/03/18 19:30:37 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "atom.h"
#include "hashmap.h"
#include "ast.h"

int	main(int argc, char **argv, char **envp)
{
	t_token_list	*tokens;
	t_lexer			lexer;
	t_hashmap		*hashmap;
	t_ast_node		*ast_node;
	char			*args[2];
	tokens = new_token_list();
	args[0] = "Hello";
	args[1] = NULL;
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
	ast_node = new_node((t_data){.simple_cmd = {.name = "echo", .args = args}});
	ast_add_parent(&ast_node, new_node((t_data){.simple_cmd = {.name = "cat", .args = args}}));
	ast_add_left(ast_node, new_node((t_data){.simple_cmd = {.name = "grav", .args = args}}));
	ast_add_right(ast_node, new_node((t_data){.simple_cmd = {.name = "grav", .args = args}}));
	printf("\t%s\n", ast_node->data.simple_cmd.name);
	printf("%s\t\t%s\n", ast_node->left->data.simple_cmd.name, ast_node->right->data.simple_cmd.name);
	// while (1)
	// {
	// 	init_lexer(&lexer);
	// 	get_tokens(tokens, lexer);
	// 	print_tokens(tokens);
	// 	printf("Size: %zu\n", tokens->num_tokens);
	// 	if (is_string_type(tokens->first->type) && \
	// 		ft_strncmp(tokens->first->value.word.value, "exit", 5) == 0)
	// 	{
	// 		free_token_list(tokens);
	// 		free(tokens);
	// 		free(lexer.input);
	// 		system("leaks minishell");
	// 		exit(0);
	// 	}
	// 	free(lexer.input);
	// 	free_token_list(tokens);
	// }
	return (0);
}
