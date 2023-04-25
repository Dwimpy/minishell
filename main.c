/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/05 22:15:18 by dwimpy            #+#    #+#             */
/*   Updated: 2023/04/25 00:36:12 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <stdio.h>
#include <stdlib.h>
// #include <readline/readline.h>
// #include <readline/history.h>
#include "readline/readline.h"
#include "readline/history.h"
#include "parser.h"
#include "tokenizer.h"
#include "environment_handler.h"

#include "x_execution.h"
#include "get_next_line.h"
#include <curses.h>
#include <term.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <signal.h>
#include <pwd.h>

int	main(int argc, char **argv, char **envp)
{
	t_input				input;
	int					fd;
	int					exit_code;

	init_input(&input, envp);
	rl_initialize();
	while (1)
	{
		if (gen_input(&input))
			continue ;
		if (parse_all_input(&input))
			continue ;
		fd = 0;
		exit_code = ft_execution(&input, input.root, &fd);
		if (exit_code >= 256)
			exit_code = exit_code / 256;
		free(hashmap_put(input.special_sym, "EXITSTATUS", ft_itoa(exit_code)));
		ast_del_node(input.root);
		input.root = NULL;
		free(input.lexer.input);
		free_token_list(input.tokens);
		free_heredoc_list(input.heredoc_files);
	}
	return (ft_atoi(hashmap_get(input.special_sym, "EXITSTATUS")));
}
