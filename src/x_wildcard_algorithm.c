/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   x_wildcard_algorithm.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkilling <tkilling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 17:12:02 by tkilling          #+#    #+#             */
/*   Updated: 2023/04/25 22:55:43 by tkilling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "x_execution.h"
#include "sys/types.h"
#include "sys/wait.h"
#include "sys/stat.h"

int		ft_get_dir(DIR **dir, struct dirent **new_dir);
void	ft_if_arr(t_wild *wild, char *str, char **arr, char new_arr[1024][256]);

void	ft_check(char *str, char **arr, char new_arr[1024][256], size_t *count)
{
	t_wild	wild;

	wild.count = count;
	if (ft_get_dir(&(wild.dir), &(wild.new_dir)))
		return ;
	wild.c_cp = *count;
	while (wild.new_dir != NULL)
	{
		wild.i = 0;
		wild.a = 0;
		wild.b = 0;
		if (arr[0] == NULL)
		{
			if (wild.new_dir->d_name[0] != '.')
				ft_strlcpy(new_arr[(*count)++], wild.new_dir->d_name, 256);
		}
		else if (!(wild.new_dir->d_name[wild.i] != arr[wild.a][wild.b]
			&& str[0] != '*' && wild.a == 0))
			ft_if_arr(&wild, str, arr, new_arr);
		wild.new_dir = readdir(wild.dir);
	}
	if (*count == wild.c_cp)
		ft_strlcpy(new_arr[(*count)++], str, 256);
	closedir(wild.dir);
}

void	ft_if_arr(t_wild *wild, char *str, char **arr, char new_arr[1024][256])
{
	wild->j = 0;
	while (arr[wild->a] != NULL && wild->new_dir->d_name[wild->j] != '\0')
	{
		wild->i = wild->j;
		wild->b = 0;
		while (arr[wild->a][wild->b] != '\0' &&
			wild->new_dir->d_name[wild->i] == arr[wild->a][wild->b] &&
			++(wild->b))
			wild->i++;
		if (arr[wild->a][wild->b] == '\0' && ((arr[wild->a + 1] == NULL &&
			str[ft_strlen(str) - 1] == '*') || (arr[wild->a + 1] == NULL &&
				wild->new_dir->d_name[wild->i] == '\0')))
			wild->a++;
		else if (arr[wild->a][wild->b] == '\0' && arr[wild->a + 1] != NULL)
			wild->a++;
		if (arr[wild->a] == NULL)
			if (wild->new_dir->d_name[0] != '.' || str[0] == '.')
				if (wild->new_dir->d_name[wild->i] == arr[wild->a - 1][wild->b]
					|| str[ft_strlen(str) - 1] == '*')
					ft_strlcpy(new_arr[(*(wild->count))++],
						wild->new_dir->d_name, 256);
		if (arr[wild->a] != NULL && wild->new_dir->d_name[wild->j] != '\0')
			(wild->j)++;
	}
}

int	ft_get_dir(DIR **dir, struct dirent **new_dir)
{
	char	*path;

	path = getcwd(NULL, 0);
	*dir = opendir(path);
	free(path);
	*new_dir = readdir(*dir);
	if (*new_dir == NULL)
		return (1);
	return (0);
}
