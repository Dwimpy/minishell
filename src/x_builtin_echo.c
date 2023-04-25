/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   x_builtin_echo.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkilling <tkilling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 18:34:03 by tkilling          #+#    #+#             */
/*   Updated: 2023/04/24 18:35:01 by tkilling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "x_execution.h"
#include <string.h>
#include <errno.h>

void	ft_handle_n_flags(char **str_arr, int *n_flag, size_t *i, size_t *j);

int	ft_echo(char **str_arr, int fd)
{
	int		n_flag;
	size_t	i;
	size_t	j;

	i = 1;
	n_flag = 0;
	ft_handle_n_flags(str_arr, &n_flag, &i, &j);
	while (str_arr[i++] != NULL)
	{
		write(fd, str_arr[i - 1], ft_strlen(str_arr[i - 1]));
		if (str_arr[i] != NULL)
			write(fd, " ", 1);
	}
	if (!n_flag)
		write(fd, "\n", 1);
	rl_on_new_line();
	return (0);
}

void	ft_handle_n_flags(char **str_arr, int *n_flag, size_t *i, size_t *j)
{
	while (str_arr[*i] != NULL && !ft_memcmp(str_arr[*i], "-n", 2))
	{
		*j = 1;
		while (str_arr[*i][*j] == 'n')
			(*j)++;
		if (str_arr[*i][*j] == '\0')
		{
			(*i)++;
			*n_flag = 1;
		}
		else
			break ;
	}
}
