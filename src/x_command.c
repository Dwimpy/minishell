/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   x_command.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkilling <tkilling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 16:15:29 by tkilling          #+#    #+#             */
/*   Updated: 2023/04/08 11:56:48 by tkilling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "x_execution.h"

static int	ft_execute(char *path, char **str_arr, t_input *input);
static void	ft_executable(char **str_arr, t_input *input);

int	ft_command(char **str_arr, t_input *input)
{
	
	char	*ptr;
	char	*path;
	char	**paths;
	size_t	i;
	int		fd_stdin;

	if (!str_arr)
		return (1);
	if (!(ft_memcmp("cd", str_arr[0], 3)))
		ft_cd(str_arr, input);
	else if (!(ft_memcmp("pwd", str_arr[0], 4)))
		ft_pwd(str_arr);
	else if (!(ft_memcmp("exit", str_arr[0], 5)))
		ft_exit(str_arr, input);
	else if (!(ft_memcmp("env", str_arr[0], 4)))
		ft_env(str_arr, input);
	else if (!(ft_memcmp("echo", str_arr[0], 5)))
		ft_echo(str_arr, 1);
	else if (!(ft_memcmp("export", str_arr[0], 7)))
		ft_export(str_arr, input);
	else if (!(ft_memcmp("unset", str_arr[0], 6)))
		ft_unset(str_arr, input);
	else if (!(ft_memcmp("./", str_arr[0], 2)) || !(ft_memcmp("../", str_arr[0], 3)))
		ft_executable(str_arr, input);
	else
	{
		path = hashmap_get(input->hashmap, "PATH");
		paths = ft_split(path, ':');
		i = 0;
		while (paths && paths[i])
		{
			ptr = ft_strjoin(paths[i], "/");
			free(paths[i]);
			path = ft_strjoin(ptr, str_arr[0]);
			free(ptr);
			if (ft_execute(path, str_arr, input))
				return (0);
			free(path);
			i++;
		}
		free(paths);
	}
	return (0);
}

int	ft_execute(char *path, char **str_arr, t_input *input)
{
	int		pid;
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
		waitpid(pid, NULL, 0);
		return (1);
	}
	else
		return(0);
}

void	ft_executable(char **str_arr, t_input *input)
{
	int		pid;
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
		waitpid(pid, NULL, 0);
	}
	else
	{
		printf("no such file or directory: %s\n", str_arr[0]);
	}
}

