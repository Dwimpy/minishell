/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   x_builtin_cd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 18:07:59 by tkilling          #+#    #+#             */
/*   Updated: 2023/04/26 13:45:54 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "x_execution.h"
#include <string.h>
#include <errno.h>

int	ft_cd_to_homedir(t_input *input, char **str_arr, char **old);
int	ft_no_dir_error(char **str_arr);
int	ft_old_pwd(t_input *input, char **str_arr, char **old);

int	ft_cd(char **str_arr, t_input *input)
{
	int		return_code;
	char	*path;
	char	*old;

	if (str_arr[1] == NULL || !(ft_memcmp("~", str_arr[1], 1)))
		return (ft_cd_to_homedir(input, &str_arr[1], &old));
	if (!ft_strncmp(str_arr[1], "-", 2))
	{
		return_code = ft_old_pwd(input, str_arr, &old);
		if (return_code != -1)
			return (return_code);
	}
	else if (chdir(str_arr[1]) == -1)
		return (ft_no_dir_error(str_arr));
	path = getcwd(NULL, 0);
	old = hashmap_put(input->hashmap, "PWD", ft_strdup(path));
	free(hashmap_put(input->hashmap, "OLDPWD", old));
	free(path);
	return (0);
}

int	ft_old_pwd(t_input *input, char **str_arr, char **old)
{
	char	*path;

	if (!(char *)hashmap_get(input->hashmap, "OLDPWD"))
	{
		ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2);
		return (1);
	}
	else if ((char *)hashmap_get(input->hashmap, "OLDPWD"))
	{
		if (chdir((char *)hashmap_get(input->hashmap, "OLDPWD")) == -1)
			return (ft_no_dir_error(str_arr));
		path = getcwd(NULL, 0);
		*old = hashmap_put(input->hashmap, "PWD", ft_strdup(path));
		ft_putstr_fd((char *)hashmap_get(input->hashmap, "OLDPWD"), 1);
		free(hashmap_put(input->hashmap, "OLDPWD", *old));
		ft_putstr_fd("\n", 1);
		free(path);
		return (0);
	}
	return (-1);
}

int	ft_no_dir_error(char **str_arr)
{
	ft_putstr_fd("minishell: cd: ", 2);
	ft_putstr_fd(str_arr[1], 2);
	ft_putendl_fd(" Not a directory", 2);
	return (1);
}

int	ft_cd_to_homedir(t_input *input, char **str_arr, char **old)
{
	char	*str;
	char	*old_str;

	str = NULL;
	str = (char *)hashmap_get(input->hashmap, "HOME");
	if (!str || str[0] == '\0')
		str = getcwd(NULL, 0);
	old_str = (*str_arr);
	(*str_arr) = ft_strjoin(str, &old_str[1]);
	free(old_str);
	*old = hashmap_put(input->hashmap, "PWD", ft_strdup(*str_arr));
	free(hashmap_put(input->hashmap, "OLDPWD", *old));
	if (*str_arr)
	{
		if (chdir(*str_arr) == -1)
		{
			ft_putstr_fd("cd: ", 2);
			ft_putstr_fd(strerror(errno), 2);
			ft_putstr_fd(": ", 2);
			ft_putstr_fd(*str_arr, 2);
			ft_putstr_fd("\n", 2);
			return (1);
		}
	}
	return (0);
}
