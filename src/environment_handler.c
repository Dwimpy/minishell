/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment_handler.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 19:42:26 by arobu             #+#    #+#             */
/*   Updated: 2023/04/07 15:29:49 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "environment_handler.h"

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
	// ft_printf("%s\n", (char *)hashmap_get(hashmap, "USER"));
}

char	**hashmap_tochar(t_hashmap *hashmap)
{
	char	**arr;
	t_entry	*entry;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (!hashmap && !hashmap->table)
		return (NULL);
	arr = (char **)malloc(sizeof(char *) * hashmap->length);
	if (!arr)
		return (NULL);
	while (i < hashmap->size)
	{
		entry = hashmap->table[i];
		while (entry)
		{
			arr[j] = (char *)malloc(sizeof(char) * \
				(ft_strlen((char *)entry->key) + ft_strlen(entry->value) + 2));
			ft_strcpy(arr[j], (char *)entry->key);
			ft_strcat(arr[j], "=");
			ft_strcat(arr[j], (char *)entry->value);
			j++;
			entry = entry->next;
		}
		i++;
	}
	return (arr);
}
