/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   x_error.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkilling <tkilling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 16:49:51 by tkilling          #+#    #+#             */
/*   Updated: 2023/04/24 16:52:54 by tkilling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "x_execution.h"

int	ft_directory_error(char **str_arr, int *status)
{
	ft_put_error_n(str_arr, ": is a directory", 0);
	if (str_arr[0][0] == '.')
	{
		if (str_arr[0][1] == '.')
			*status = 127;
		else if (str_arr[0][1] == '/')
			*status = 126;
		else if (str_arr[0][1] == '\0')
		{
			ft_putstr_fd("minishell: .: filename argument required\n",
				2);
			ft_putendl_fd(".: usage: . filename [arguments]", 2);
			*status = 127;
		}
		else
			*status = 127;
		return (*status);
	}
	else if (str_arr[0][0] == '/')
		*status = 126;
	return (0);
}

void	ft_cmd_not_found(char **str_arr, int *status)
{
	if (*status == -1)
	{
		ft_putstr_fd("minishell: command not found: ", 2);
		ft_putstr_fd(str_arr[0], 2);
		ft_putstr_fd("\n", 2);
		*status = 127;
	}
}

int	ft_put_error(char **str_arr, char *str, int status)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(str_arr[0], 2);
	ft_putendl_fd(str, 2);
	return (status);
}

int	ft_put_error_n(char **str_arr, char *str, int status)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(str_arr[0], 2);
	ft_putendl_fd(str, 2);
	ft_putstr_fd("\n", 2);
	return (status);
}
