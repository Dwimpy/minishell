/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   x_command.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkilling <tkilling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 16:15:29 by tkilling          #+#    #+#             */
/*   Updated: 2023/04/25 21:45:12 by tkilling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "x_execution.h"
#include "sys/types.h"
#include "sys/wait.h"
#include "sys/stat.h"

void	ft_try_paths(int *status, t_input *input, char **str_arr);
void	ft_check_paths(int *status, t_input *input, char **str_arr);
int		ft_which_command(char **str_arr, t_input *input);
int		ft_if_tilde(char **str_arr, t_input *input, t_ast_node *root);

int	ft_command(char **str_arr, t_input *input, t_ast_node *root)
{
	int		status;
	int		stdin_cp;
	int		stdout_cp;
	char	**new_arr;

	if (!str_arr)
		return (1);
	stdin_cp = -1;
	stdout_cp = -1;
	expand_env_vars(root->data.command.cmd.args, input);
	new_arr = ft_wildcard(str_arr, root);
	if (ft_if_tilde((char **)new_arr, input, root) != 0)
		return (126);
	if (ft_redirect(root, &stdin_cp, &stdout_cp))
		return (1);
	status = ft_which_command((char **)new_arr, input);
	ft_redirect_back(&stdin_cp, &stdout_cp);
	return (status);
}

int	ft_which_command(char **str_arr, t_input *input)
{
	int		status;

	if (!(ft_memcmp("cd", str_arr[0], 3)))
		status = ft_cd(str_arr, input);
	else if (!(ft_memcmp("pwd", str_arr[0], 4)))
		status = ft_pwd();
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
	else if (!(ft_memcmp("./", str_arr[0], 2))
		|| !(ft_memcmp("../", str_arr[0], 3)))
		status = ft_executable(str_arr, input);
	else
		ft_check_paths(&status, input, str_arr);
	return (status);
}

void	ft_check_paths(int *status, t_input *input, char **str_arr)
{
	*status = -1;
	if (!(char *)hashmap_get(input->hashmap, "PATH")
		|| ((char *)hashmap_get(input->hashmap, "PATH"))[0] == '\0')
		*status = ft_executable(str_arr, input);
	else if (is_directory(str_arr[0]))
	{
		if (ft_directory_error(str_arr, status) != 0)
			return ;
	}
	else if (str_arr[0][0] == '/' && access(str_arr[0], F_OK) != 0)
	{
		*status = ft_put_error_n(str_arr, ": No such file or directory", 127);
	}
	else if (access(str_arr[0], F_OK) == 0)
	{
		if (access(str_arr[0], X_OK) == 0)
			*status = ft_execute(str_arr[0], str_arr, input);
	}
	else
		ft_try_paths(status, input, str_arr);
	ft_cmd_not_found(str_arr, status);
}

void	ft_try_paths(int *status, t_input *input, char **str_arr)
{
	char	*ptr;
	char	*path;
	char	**paths;
	size_t	i;

	i = 0;
	path = hashmap_get(input->hashmap, "PATH");
	paths = ft_split(path, ':');
	while (paths && paths[i])
	{
		ptr = ft_strjoin(paths[i], "/");
		free(paths[i]);
		path = ft_strjoin(ptr, str_arr[0]);
		free(ptr);
		if (*status == -1)
			*status = ft_execute(path, str_arr, input);
		free(path);
		i++;
	}
	free(paths);
}

int	ft_if_tilde(char **str_arr, t_input *input, t_ast_node *root)
{
	char	*new;

	if (str_arr[0][0] == '~')
	{
		new = ft_calloc(ft_strlen((char *)hashmap_get(input->special_sym,
						"TILDE")) + ft_strlen(str_arr[0]), sizeof(char));
		new = ft_strcpy(new, (char *)hashmap_get(input->special_sym, "TILDE"));
		new = ft_strcat(new, &root->data.command.cmd.args[0][1]);
		free(root->data.command.cmd.args[0]);
		str_arr[0] = new;
		if (is_directory(new))
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(new, 2);
			ft_putstr_fd(": is a directory", 2);
			ft_putstr_fd("\n", 2);
			return (126);
		}
	}
	return (0);
}
