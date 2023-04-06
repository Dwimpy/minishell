/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/05 22:15:18 by dwimpy            #+#    #+#             */
/*   Updated: 2023/04/06 20:41:15 by arobu            ###   ########.fr       */
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
		gen_input(&input);
		// if (generate_input(&input) != 0)
		// 	continue ;
		// parse_all_input(&input);
		free(input.lexer.input);
	}
	return (0);
}
