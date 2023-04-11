/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/05 22:15:18 by dwimpy            #+#    #+#             */
/*   Updated: 2023/04/11 22:11:43 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "tokenizer.h"
#include "environment_handler.h"

int	main(int argc, char **argv, char **envp)
{
	t_input			input;
	char			**arr;

	init_input(&input, envp);
	// free(hashmap_put(input.hashmap, "USER", ft_strdup("HELLO")));
	// arr = hashmap_tochar(input.hashmap);
	// while (*arr)
	// {
	// 	printf("%s\n", *arr);
	// 	arr++;
	// }
	while (1)
	{
		if (gen_input(&input) == 1)
			continue ;
		if (input.unexpected != 0)
			printf("syntax error near: %d\n", input.unexpected);
		else
			parse_all_input(&input);
		ast_del_node(input.root);
		input.root = NULL;
		free(input.lexer.input);
		free_token_list(input.tokens);
		// readline("what >");
		// rl_set_prompt("");
		// rl_on_new_line();
		// rl_replace_line("", 0);
		// rl_redisplay();

	}
	return (0);
}
