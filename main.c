/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/05 22:15:18 by dwimpy            #+#    #+#             */
/*   Updated: 2023/03/17 17:35:51 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "atom.h"
#include "hashmap.h"

int	main(void)
{
	t_token_list	*tokens;
	t_token			*token;
	t_lexer			lexer;
	char			*line;
	t_hashmap		*hashmap;
	char			**array;
	
	int	a = 25;
	int	b = 22;
	tokens = new_token_list();
	add_token(tokens, new_token(TOKEN_WORD, "Hello"));
	hashmap = hashmap_new(100);
	printf("%02x\n", table_hash(tokens));
	hashmap_put(hashmap, tokens, &a);
	hashmap_put(hashmap, "key", "WTF");
	hashmap_put(hashmap, "asd", &b);
	hashmap_put(hashmap, "fds", &b);
	printf("Prev elem: %d", *(int *)hashmap_put(hashmap, tokens, &b));
	hashmap_put(hashmap, tokens, &b);
	hashmap_put(hashmap, tokens, &b);
	hashmap_put(hashmap, tokens, &b);
	array = (char **)hashmap_toarray(hashmap, NULL);
	printf("\n%s\n", array[1]);
	printf("Elem: %d\t", *(int *)hashmap_get(hashmap, tokens));
	printf("%d", hashmap_length(hashmap));
	// while (1)
	// {
	// 	line = readline("minishell$ ");
	// 	if (!line)
	// 		break ;
	// 	init_lexer(&lexer, line);
	// 	printf("Line: %s\n", line);
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
	// 		system("leaks minishell");
	// 		exit(0);
	// 	}
	// 	free(lexer.input);
	// 	free_token_list(tokens);
	// 	free(line);
	// }
	return (0);
}
