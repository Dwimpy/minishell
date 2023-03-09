/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dwimpy <dwimpy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/05 22:15:18 by dwimpy            #+#    #+#             */
/*   Updated: 2023/03/08 22:35:16 by dwimpy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include <readline/readline.h>
#include <readline/history.h>

int main(void)
{
	t_lexer *lexer;
	t_token *token;
	char *line;

	lexer = init_lexer("   echo     hello \"world\" | new command");
    while (1)
    {
        line = readline("minishell$ ");
        if (!line)
            break ;
        token = get_next_token(lexer);
        printf("Token value: [%s]\n", token->value);
        add_history(line);
        free(line);
    }
    return (0);
}