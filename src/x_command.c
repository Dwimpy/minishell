/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   x_command.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 16:15:29 by tkilling          #+#    #+#             */
/*   Updated: 2023/04/16 15:27:18 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "x_execution.h"
#include "sys/types.h"
#include "sys/wait.h"

static int	ft_execute(char *path, char **str_arr, t_input *input);
static int	ft_executable(char **str_arr, t_input *input);
static int	ft_redirect(t_ast_node *root, int *stdin_cp, int *stdout_cp);
static void	ft_redirect_back(t_ast_node *root, int *stdin_cp, int *stdout_cp);

int	ft_command(char **str_arr, t_input *input, t_ast_node *root)
{
	
	char	*ptr;
	char	*path;
	char	**paths;
	size_t	i;
	int		status;

	int		stdin_cp;
	int		stdout_cp;

	if (!str_arr)
		return (1);
	stdin_cp = -1;
	stdout_cp = -1;
	if (ft_redirect(root, &stdin_cp, &stdout_cp))
		return (1);
	if (!(ft_memcmp("cd", str_arr[0], 3)))
		status = ft_cd(str_arr, input);
	else if (!(ft_memcmp("pwd", str_arr[0], 4)))
		status = ft_pwd(str_arr);
	else if (!(ft_memcmp("exit", str_arr[0], 5)))
		status = ft_exit(str_arr, input);
	else if (!(ft_memcmp("env", str_arr[0], 4)))
		status = ft_env(str_arr, input);
	else if (!(ft_memcmp("echo", str_arr[0], 5)))
		status = ft_echo(str_arr, 1);
	else if (!(ft_memcmp("export", str_arr[0], 7)))
		status = ft_export(str_arr, input);
	else if (!(ft_memcmp("unset", str_arr[0], 6)))
		status = ft_unset(str_arr, input);
	else if (!(ft_memcmp("./", str_arr[0], 2)) || !(ft_memcmp("../", str_arr[0], 3)))
		status = ft_executable(str_arr, input);
	else
	{
		status = -1;
		i = 0;
		if (access(str_arr[0], F_OK) == 0)
		{
			status = ft_execute(str_arr[0], str_arr, input);
		}
		else
		{
			path = hashmap_get(input->hashmap, "PATH");
			paths = ft_split(path, ':');
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
		}
		if (status == -1)
		{

			ft_putstr_fd("minishell: command not found: ", 2);
			ft_putstr_fd(str_arr[0], 2);
			ft_putstr_fd("\n", 2);
			return (-2);
		}
	}
	ft_redirect_back(root, &stdin_cp, &stdout_cp);
	return (status);
}

int	ft_redirect(t_ast_node *root, int *stdin_cp, int *stdout_cp)
{
	int		fd;

	if (root->data.command.output.filename)
	{
		fd = open(root->data.command.output.filename, O_TRUNC | O_WRONLY, 0644);
		if (fd < 0)
			return (1);
		else
		{
			*stdout_cp = dup(STDOUT_FILENO);
			close(STDOUT_FILENO);
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
	}
	if (root->data.command.input.filename)
	{
		fd = open(root->data.command.input.filename, O_RDONLY);
		if (fd < 0)
			return (1);
		else
		{
			*stdin_cp = dup(STDIN_FILENO);
			close(STDIN_FILENO);
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
	}
	return (0);
}

void	ft_redirect_back(t_ast_node *root, int *stdin_cp, int *stdout_cp)
{	
	// printf("input :%s\n", root->data.command.input.filename);
	// printf("output :%s\n", root->data.command.output.filename);
	if (*stdout_cp >= 0)
	{
		close (STDOUT_FILENO);
		dup2(*stdout_cp, STDOUT_FILENO);
		close(*stdout_cp);
	}
	if (*stdin_cp >= 0)
	{
		close (STDIN_FILENO);
		dup2(*stdin_cp, STDIN_FILENO);
		close(*stdin_cp);
	}
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

