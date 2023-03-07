/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dwimpy <dwimpy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/05 22:15:18 by dwimpy            #+#    #+#             */
/*   Updated: 2023/03/06 17:17:25 by dwimpy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

int main(void)
{
    char    *line;
    while (1)
    {
        line = readline("minishell$ ");
        if (!line)
            break ;
        printf("Line: %s\n", line);
        add_history(line);
        free(line);
    }
    return (0);
}