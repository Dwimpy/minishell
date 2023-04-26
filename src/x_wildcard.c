/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   x_wildcard.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 13:22:24 by tkilling          #+#    #+#             */
/*   Updated: 2023/04/26 13:04:19 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "x_execution.h"
#include "sys/types.h"
#include "sys/wait.h"
#include "sys/stat.h"

int		ft_if_wildcard(char *str, char new_arr[1024][256], size_t *count);
void	ft_check(char *str, char **arr, char new_arr[1024][256], size_t *count);
void	ft_get_arr(char *str, char new_arr[1024][256], size_t *count);
char	**ft_cp_to_arr(size_t count, char new_arr[1024][256]);

char	**ft_wildcard(char **str_arr, t_ast_node *root)
{
	size_t	i;
	size_t	count;
	char	**arr;
	char	new_arr[1024][256];

	i = 0;
	count = 0;
	while (str_arr[i] != NULL)
		ft_get_arr(str_arr[i++], new_arr, &count);
	arr = ft_cp_to_arr(count, new_arr);
	i = 0;
	while (str_arr[i])
		free(str_arr[i++]);
	free(str_arr);
	root->data.command.cmd.args = arr;
	return (arr);
}

char	**ft_cp_to_arr(size_t count, char new_arr[1024][256])
{
	size_t	i;
	size_t	j;
	char	**arr;

	i = 0;
	arr = (char **)malloc(sizeof(char *) * (count + 1));
	arr[count] = NULL;
	while (i < count)
	{
		j = 0;
		arr[i] = malloc(sizeof(char) * (1 + ft_strlen(new_arr[i])));
		while (new_arr[i][j] != '\0')
		{
			arr[i][j] = new_arr[i][j];
			j++;
		}
		arr[i][j] = new_arr[i][j];
		i++;
	}
	return (arr);
}

void	ft_get_arr(char *str, char new_arr[1024][256], size_t *count)
{
	char			**arr;
	size_t			i;

	if (ft_if_wildcard(str, new_arr, count) == 0)
		return ;
	arr = ft_split(str, '*');
	if (arr == NULL)
		return ;
	ft_check(str, arr, new_arr, count);
	i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

int	ft_if_wildcard(char *str, char new_arr[1024][256], size_t *count)
{
	size_t	i;

	i = 0;
	while (str[i] != '\0' && str[i] != '*')
		i++;
	if (str[i] == '\0')
	{
		ft_strlcpy(new_arr[(*count)++], str, 256);
		return (0);
	}
	return (1);
}
