/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   x_command.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkilling <tkilling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 16:15:29 by tkilling          #+#    #+#             */
/*   Updated: 2023/04/09 14:19:03 by tkilling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "x_execution.h"

static int	ft_execute(char *path, char **str_arr, t_input *input);
static int	ft_executable(char **str_arr, t_input *input);

int	ft_command(char **str_arr, t_input *input)
{
	
	char	*ptr;
	char	*path;
	char	**paths;
	size_t	i;
	int		status;

	if (!str_arr)
		return (1);
	if (!(ft_memcmp("cd", str_arr[0], 3)))
		i = ft_cd(str_arr, input);
	else if (!(ft_memcmp("pwd", str_arr[0], 4)))
		i = ft_pwd(str_arr);
	else if (!(ft_memcmp("exit", str_arr[0], 5)))
		i = ft_exit(str_arr, input);
	else if (!(ft_memcmp("env", str_arr[0], 4)))
		i = ft_env(str_arr, input);
	else if (!(ft_memcmp("echo", str_arr[0], 5)))
		i = ft_echo(str_arr, 1);
	else if (!(ft_memcmp("export", str_arr[0], 7)))
		i = ft_export(str_arr, input);
	else if (!(ft_memcmp("unset", str_arr[0], 6)))
		i = ft_unset(str_arr, input);
	else if (!(ft_memcmp("./", str_arr[0], 2)) || !(ft_memcmp("../", str_arr[0], 3)))
		i = ft_executable(str_arr, input);
	else
	{
		status = -1;
		path = hashmap_get(input->hashmap, "PATH");
		paths = ft_split(path, ':');
		i = 0;
		while (paths && paths[i])
		{
			ptr = ft_strjoin(paths[i], "/");
			free(paths[i]);
			path = ft_strjoin(ptr, str_arr[0]);
			free(ptr);
			if (status == -1)
				status = ft_execute(path, str_arr, input);
			free(path);
			i++;
		}
		free(paths);
		return (status);
	}
	return (i);
}

int	ft_execute(char *path, char **str_arr, t_input *input)
{
	int		pid;
	int		status;
	char 	**hashmap;

	if (access(path, F_OK) == 0)
	{
		pid = fork();
		if (pid == -1)
			exit(1);
		if (pid == 0)
		{
			hashmap = hashmap_tochar(input->hashmap);
			if (execve(path, str_arr, hashmap))
				exit(1);
			exit(0);
		}
		else
			waitpid(pid, &status, 0);
		return (status);
	}
	else
		return(-1);
}

int	ft_executable(char **str_arr, t_input *input)
{
	int		pid;
	int		status;
	char 	**hashmap;

	if (access(str_arr[0], F_OK) == 0)
	{
		pid = fork();
		if (pid == -1)
			exit(1);
		if (pid == 0)
		{
			hashmap = hashmap_tochar(input->hashmap);
			if (execve(str_arr[0], str_arr, hashmap))
				exit(1);
			exit(0);
		}
		waitpid(pid, &status, 0);
		return (status);
	}
	else
	{
		printf("no such file or directory: %s\n", str_arr[0]);
		return (1);
	}
}

