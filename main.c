/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/05 22:15:18 by dwimpy            #+#    #+#             */
/*   Updated: 2023/04/25 17:36:42 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
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
		free_loop(&input, &exit_code);
	}
	return (ft_atoi(hashmap_get(input.special_sym, "EXITSTATUS")));
}
