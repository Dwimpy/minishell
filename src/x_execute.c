/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   x_execute.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkilling <tkilling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 14:15:08 by tkilling          #+#    #+#             */
/*   Updated: 2023/04/24 14:15:50 by tkilling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "x_execution.h"
// #include "sys/types.h"
// #include "sys/wait.h"
// #include "sys/stat.h"

int	ft_execute(char *path, char **str_arr, t_input *input)
{
	int		pid;
	int		status;
	char	**hashmap;

	if (access(path, F_OK) == 0)
	{
		pid = fork();
		if (pid == -1)
			exit(1);
		if (pid == 0)
		{
			ft_signals_child(&(input->sa));
			hashmap = hashmap_tochar(input->hashmap);
			if (execve(path, str_arr, hashmap))
				exit(1);
		}
		else
			waitpid(pid, &status, 0);
		return (WEXITSTATUS(status));
	}
	else
		return (-1);
}
