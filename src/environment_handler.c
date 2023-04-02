/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment_handler.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 19:42:26 by arobu             #+#    #+#             */
/*   Updated: 2023/04/02 20:54:43 by arobu            ###   ########.fr       */
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
	const void		*key;
	char			*value;
	char			*equal;
	size_t			len;

	env = envp;
	while (*env)
	{
		len = ft_strlen(*env);
		equal = ft_strnstr(*env, "=", len);
		key = (char *)ft_substr(*env, 0, equal - *env);
		value = ft_substr(equal, 1, ft_strlen(equal) - 1);
		hashmap_put(hashmap, key, value);
		(env)++;
	}
	// ft_printf("%s\n", (char *)hashmap_get(hashmap, "USER"));
}

