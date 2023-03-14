/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/05 22:15:18 by dwimpy            #+#    #+#             */
/*   Updated: 2023/03/14 16:38:12 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>

int	main(void)
{
	t_token_list	*tokens;
	t_token			*token;
	t_lexer			lexer;
	char			*line;

	tokens = new_token_list();
	while (1)
	{
		line = readline("minishell$ ");
		if (!line)
			break ;
		init_lexer(&lexer, line);
		while (lexer.read_position < lexer.input_len)
		{
			token = get_next_token(&lexer);
			add_token(tokens, token);
		}
		print_tokens(tokens);
		printf("Size: %zu\n", tokens->num_tokens);
		add_history(line);
		if (is_string_type(tokens->first->type) && \
			ft_strncmp(tokens->first->value.word.value, "exit", 5) == 0)
		{
			free_token_list(tokens);
			free(tokens);
			system("leaks minishell");
			exit(0);
		}
		free(lexer.input);
		free_token_list(tokens);
		free(line);
	}
	return (0);
}
