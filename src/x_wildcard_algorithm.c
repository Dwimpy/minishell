/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   x_wildcard_algorithm.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkilling <tkilling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 17:12:02 by tkilling          #+#    #+#             */
/*   Updated: 2023/04/25 22:10:15 by tkilling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "x_execution.h"
#include "sys/types.h"
#include "sys/wait.h"
#include "sys/stat.h"
#include <dirent.h>

void	ft_check(char *str, char **arr, char new_arr[1024][256], size_t *count)
{
	size_t			i;
	size_t			j;
	size_t			a;
	size_t			b;
	size_t			c_cp;
	DIR				*dir;
	struct dirent	*new_dir;
	char			*path;

	path = getcwd(NULL, 0);
	dir = opendir(path);
	free(path);
	new_dir = readdir(dir);
	if (new_dir == NULL)
		return ;
	c_cp = *count;
	while (new_dir != NULL)
	{
		i = 0;
		a = 0;
		b = 0;
		if (arr[0] == NULL)
		{
			if (new_dir->d_name[0] != '.')
				ft_strlcpy(new_arr[(*count)++], new_dir->d_name, 256);
		}
		else if (!(new_dir->d_name[i] != arr[a][b] && str[0] != '*' && a == 0))
		{
			j = 0;
			while (arr[a] != NULL && new_dir->d_name[j] != '\0')
			{
				i = j;
				b = 0;
				while (arr[a][b] != '\0' && new_dir->d_name[i] == arr[a][b])
				{
					i++;
					b++;
				}
				if (arr[a][b] == '\0' && ((arr[a + 1] == NULL && str[ft_strlen(str) - 1] == '*') || (arr[a + 1] == NULL && new_dir->d_name[i] == '\0')))
					a++;
				else if (arr[a][b] == '\0' && arr[a + 1] != NULL)
					a++;
				if (arr[a] == NULL)
				{
					if (new_dir->d_name[0] != '.' || str[0] == '.')
						if (new_dir->d_name[i] == arr[a - 1][b] || str[ft_strlen(str) - 1] == '*')
							ft_strlcpy(new_arr[(*count)++], new_dir->d_name, 256);
				}
				else
					if (new_dir->d_name[j] != '\0')
						j++;
			}
		}
		new_dir = readdir(dir);
	}
	if (*count == c_cp)
		ft_strlcpy(new_arr[(*count)++], str, 256);
	closedir(dir);
}
