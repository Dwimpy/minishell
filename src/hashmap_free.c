/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashmap_free.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 00:32:25 by arobu             #+#    #+#             */
/*   Updated: 2023/04/25 00:32:31 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hashmap.h"

void	hashmap_free(t_hashmap **hashmap)
{
	t_entry	*entry;
	t_entry	*prev_entry;
	int		i;

	i = 0;
	if (!hashmap && !(*hashmap)->table)
		return ;
	if ((*hashmap)->length > 0)
	{
		while (i < (*hashmap)->size)
		{
			entry = (*hashmap)->table[i];
			while (entry)
			{
				prev_entry = entry;
				entry = entry->next;
				free((char *)prev_entry->key);
				free(prev_entry->value);
				free(prev_entry);
			}
			i++;
		}
	}
	free((*hashmap)->table);
	free(*hashmap);
}
