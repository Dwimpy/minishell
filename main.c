/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/05 22:15:18 by dwimpy            #+#    #+#             */
/*   Updated: 2023/03/18 12:32:03 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "atom.h"
#include "hashmap.h"

int	main(int argc, char **argv, char **envp)
{
	t_token_list	*tokens;
	t_token			*token;
	t_lexer			lexer;
	char			*line;
	t_hashmap		*hashmap;
	char			**array;
	
	int	a = 25;
	int	b = 22;
	// tokens = new_token_list();
	hashmap = hashmap_new(100);
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
	hashmap_free(&hashmap);
	// printf("%d", hashmap_length(hashmap));
	// printf("%s", envp[0]);
	// while (1)
	// {
	// 	line = readline("minishell$ ");
	// 	if (!line)
	// 		break ;
	// 	init_lexer(&lexer, line);
	// 	while (lexer.read_position < lexer.input_len)
	// 	{
	// 		token = get_next_token(&lexer);
	// 		add_token(tokens, token);
	// 	}
	// 	print_tokens(tokens);
	// 	printf("Size: %zu\n", tokens->num_tokens);
	// 	add_history(line);
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
	// 	free(line);
	// }
	return (0);
}
