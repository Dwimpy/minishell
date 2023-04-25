/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   x_builtins.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkilling <tkilling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 15:44:49 by tkilling          #+#    #+#             */
/*   Updated: 2023/04/24 18:42:41 by tkilling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "x_execution.h"
#include <string.h>
#include <errno.h>

static int	check_valid_unset_ident(char *str, t_input *input, int *status);

int	ft_unset(char **str_arr, t_input *input)
{
	size_t	i;
	int		status;

	i = 1;
	status = 0;
	while (str_arr[i] != NULL)
	{
		if (check_valid_unset_ident(str_arr[i], input, &status))
		{
			i++;
			continue ;
		}
		hashmap_remove(input->hashmap, str_arr[i++]);
	}
	return (status);
}

int	ft_str_check(char *str, t_input *input, int *status)
{
	if (str && str[0] == '\0')
	{
		*status = 1;
		free(hashmap_put(input->hashmap, "EXITSTATUS", ft_itoa(*status)));
		return (*status);
	}
	return (0);
}

int	check_valid_unset_ident(char *str, t_input *input, int *status)
{
	int		j;
	char	*copy;

	if (ft_str_check(str, input, status) != 0)
		return (*status);
	copy = ft_strdup(str);
	j = 0;
	while (copy[j])
	{
		if (ft_strchr("@%^*#-!&=?$~.{}+", copy[j]))
		{
			ft_putstr_fd("minishell: export: `", 2);
			ft_putstr_fd(copy, 2);
			ft_putendl_fd("': not a valid identifier", 2);
			if (copy[0] == '-')
				*status = 2;
			else
				*status = 1;
			free(hashmap_put(input->hashmap, "EXITSTATUS", ft_itoa(*status)));
			free(copy);
			return (*status);
		}
		j++;
	}
	return (0);
}

int	ft_env(char **str_arr, t_input *input)
{
	char	**arr;
	size_t	i;

	i = 0;
	arr = hashmap_tochar(input->hashmap);
	if (!(char *)hashmap_get(input->hashmap, "PATH"))
	{
		ft_putstr_fd("minishell: env: No such file or directory\n", 2);
		return (1);
	}
	if (str_arr[1] && !ft_memcmp(str_arr[1], "-i", 3))
	{
		if (str_arr[2] && (!ft_memcmp(str_arr[2], "./", 2)
				|| !ft_memcmp(str_arr[2], "../", 3)))
			return (ft_executable_no_env(&str_arr[2], input));
		return (1);
	}
	while (arr[i] != NULL)
	{
		printf("%s\n", arr[i]);
		free(arr[i]);
		i++;
	}
	free(arr);
	return (0);
}

int	ft_pwd(void)
{
	char	*path;

	path = getcwd(NULL, 0);
	ft_putendl_fd(path, 1);
	free(path);
	return (0);
}
