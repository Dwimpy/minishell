/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   x_executable.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkilling <tkilling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 13:37:22 by tkilling          #+#    #+#             */
/*   Updated: 2023/04/24 16:52:48 by tkilling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "x_execution.h"
#include "sys/types.h"
#include "sys/wait.h"
#include "sys/stat.h"

int	ft_executable(char **str_arr, t_input *input)
{
	int		pid;
	int		status;

	status = 0;
	if (is_directory(str_arr[0]))
		status = ft_put_error_n(str_arr, ": is a directory", 126);
	else if (access(str_arr[0], F_OK) == 0 && access(str_arr[0], X_OK) == 0)
	{
		pid = fork();
		if (pid == -1)
			exit(1);
		if (pid == 0)
		{
			ft_signals_child(&(input->sa));
			if (execve(str_arr[0], str_arr, hashmap_tochar(input->hashmap)))
				exit(1);
		}
		waitpid(pid, &status, 0);
		return (WEXITSTATUS(status));
	}
	else if (access(str_arr[0], F_OK) != 0)
		status = ft_put_error(str_arr, ": No such file or directory", 127);
	else if (access(str_arr[0], X_OK) != 0)
		status = ft_put_error(str_arr, ": Permission denied", 126);
	return (status);
}

int	ft_executable_no_env(char **str_arr, t_input *input)
{
	int		pid;
	int		status;

	status = 0;
	if (is_directory(str_arr[0]))
		status = ft_put_error_n(str_arr, ": is a directory", 126);
	else if (access(str_arr[0], F_OK) == 0 && access(str_arr[0], X_OK) == 0)
	{
		pid = fork();
		if (pid == -1)
			exit(1);
		if (pid == 0)
		{
			ft_signals_child(&(input->sa));
			if (execve(str_arr[0], str_arr, NULL))
				exit(1);
		}
		waitpid(pid, &status, 0);
		return (WEXITSTATUS(status));
	}
	else if (access(str_arr[0], F_OK) != 0)
		status = ft_put_error(str_arr, ": No such file or directory", 127);
	else if (access(str_arr[0], X_OK) != 0)
		status = ft_put_error(str_arr, ": Permission denied", 126);
	return (status);
}

int	is_directory(const char *path)
{
	struct stat	statbuffer;

	if (stat(path, &statbuffer) != 0)
		return (0);
	return (S_ISDIR(statbuffer.st_mode));
}
