/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   x_builtin_cd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkilling <tkilling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 18:07:59 by tkilling          #+#    #+#             */
/*   Updated: 2023/04/24 18:08:53 by tkilling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "x_execution.h"
#include <string.h>
#include <errno.h>

int	ft_cd(char **str_arr, t_input *input)
{
	char	*str;
	char	*path;
	char	*old;

	str = NULL;
	if (str_arr[1] == NULL || !(ft_memcmp("~", str_arr[1], 2)))
	{
		str = (char *)hashmap_get(input->hashmap, "HOME");
		if (!str || str[0] == '\0')
		{
			str = getcwd(NULL, 0);
		}
		old = hashmap_put(input->hashmap, "PWD", ft_strdup(str));
		free(hashmap_put(input->hashmap, "OLDPWD", old));
		if (str)
		{
			if (chdir(str) == -1)
			{
				ft_putstr_fd("cd: ", 2);
				ft_putstr_fd(strerror(errno), 2);
				ft_putstr_fd(": ", 2);
				ft_putstr_fd(str, 2);
				ft_putstr_fd("\n", 2);
				return (1);
			}
		}
	}
	else
	{
		if (!ft_strncmp(str_arr[1], "-", 2))
		{
			if (!(char *)hashmap_get(input->hashmap, "OLDPWD"))
			{
				ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2);
				return (1);
			}
			else if ((char *)hashmap_get(input->hashmap, "OLDPWD"))
			{
				if (chdir((char *)hashmap_get(input->hashmap, "OLDPWD")) == -1)
				{
					ft_putstr_fd("minishell: cd: ", 2);
					ft_putstr_fd(str_arr[1], 2);
					ft_putendl_fd(" Not a directory", 2);
				}
				else
				{
					path = getcwd(NULL, 0);
					old = hashmap_put(input->hashmap, "PWD", ft_strdup(path));
					ft_putstr_fd((char *)hashmap_get(input->hashmap, "OLDPWD"), 1);
					free(hashmap_put(input->hashmap, "OLDPWD", old));
					ft_putstr_fd("\n", 1);
					free(path);
					return (0);
				}
				return (1);
			}
		}
		else if (chdir(str_arr[1]) == -1)
		{
			ft_putstr_fd("minishell: cd: ", 2);
			ft_putstr_fd(str_arr[1], 2);
			ft_putendl_fd(" Not a directory", 2);
			return (1);
		}
		path = getcwd(NULL, 0);
		old = hashmap_put(input->hashmap, "PWD", ft_strdup(path));
		free(hashmap_put(input->hashmap, "OLDPWD", old));
		free(path);
	}
	return (0);
}
