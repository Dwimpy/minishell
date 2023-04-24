/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment_handler.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 19:42:26 by arobu             #+#    #+#             */
/*   Updated: 2023/04/24 21:10:48 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "environment_handler.h"
#include <string.h>
#include <stdio.h>

static void	copy_string(char *str, void *key, void *value);

t_hashmap	*load_environment(char **envp)
{
	t_hashmap	*hashmap;

	hashmap = hashmap_new(500);
	load_data(hashmap, envp);
	return (hashmap);
}

void	load_data(t_hashmap *hashmap, char **envp)
{
	char			**env;
	char			*key;
	char			*value;
	char			*equal;
	size_t			len;

	env = envp;
	while (env && *env)
	{
		len = ft_strlen(*env);
		equal = ft_strnstr(*env, "=", len);
		key = ft_substr(*env, 0, equal - *env);
		value = ft_substr(equal, 1, ft_strlen(equal) - 1);
		hashmap_put(hashmap, key, value);
		free(key);
		(env)++;
	}
}

char	**hashmap_tochar(t_hashmap *hashmap)
{
	char	**arr;
	t_entry	*entry;
	int		i;
	int		j;

	i = -1;
	j = 0;
	if (!hashmap && !hashmap->table)
		return (NULL);
	arr = (char **)ft_calloc(hashmap->length + 1, sizeof(char *));
	if (!arr)
		return (NULL);
	while (++i < hashmap->size)
	{
		entry = hashmap->table[i];
		while (entry)
		{
			arr[j] = (char *)malloc(sizeof(char) * \
				(ft_strlen((char *)entry->key) + ft_strlen(entry->value) + 2));
			copy_string(arr[j], (char *)entry->key, (char *)entry->value);
			j++;
			entry = entry->next;
		}
	}
	return (arr);
}

static void	copy_string(char *str, void *key, void *value)
{
	ft_strcpy(str, key);
	ft_strcat(str, "=");
	ft_strcat(str, value);
}
