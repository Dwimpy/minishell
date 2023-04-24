/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashmap_operations.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/18 12:30:16 by arobu             #+#    #+#             */
/*   Updated: 2023/04/25 00:32:18 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hashmap.h"

void	free_all(void *key, void *value, void *node);

int		chech_hashmap_entry(t_hashmap *hashmap, t_entry **entry, \
			const void *key);

void	*hashmap_get(t_hashmap *hashmap, const void *key)
{
	t_entry	*entry;
	int		index;

	if (!hashmap || !hashmap->table || !key)
		return (NULL);
	index = hashmap->hash(key) % hashmap->size;
	entry = hashmap->table[index];
	while (entry)
	{
		if (hashmap->compare(key, entry->key))
			break ;
		entry = entry->next;
	}
	if (entry)
		return (entry->value);
	return (NULL);
}

void	*hashmap_put(t_hashmap *hashmap, const void *key, void *value)
{
	t_entry	*entry;
	int		index;
	void	*prev;

	if (!hashmap && !hashmap->table && !key)
		return (NULL);
	index = hashmap->hash(key) % hashmap->size;
	entry = hashmap->table[index];
	chech_hashmap_entry(hashmap, &entry, key);
	if (!entry)
	{
		entry = (t_entry *)malloc(sizeof(t_entry));
		entry->key = ft_strdup((char *)key);
		entry->value = (char *)value;
		entry->next = hashmap->table[index];
		hashmap->table[index] = entry;
		hashmap->length++;
		prev = NULL;
	}
	else
		prev = entry->value;
	entry->value = (char *)value;
	return (prev);
}

int	chech_hashmap_entry(t_hashmap *hashmap, t_entry **entry, \
			const void *key)
{
	while (*entry)
	{
		if (hashmap->compare(key, (*entry)->key))
			return (0);
		*entry = (*entry)->next;
	}
	return (0);
}

void	*hashmap_remove(t_hashmap *hashmap, const void *key)
{
	t_entry	**entry_ptr;
	t_entry	*entry;
	void	*value;
	int		index;
	int		i;

	if (!hashmap && !hashmap->table && !key)
		return (NULL);
	index = hashmap->hash(key) % hashmap->size;
	entry_ptr = &hashmap->table[index];
	i = 0;
	while (entry_ptr && (*entry_ptr))
	{
		if (hashmap->compare(key, (*entry_ptr)->key))
		{
			entry = (*entry_ptr);
			value = entry->value;
			(*entry_ptr) = entry->next;
			hashmap->length--;
			free_all((void *)entry->key, (void *)entry->value, (void *)entry);
			return (NULL);
		}
		entry_ptr = &(*entry_ptr)->next;
	}
	return (NULL);
}

void	free_all(void *key, void *value, void *node)
{
	free((void *)(key));
	free((void *)(value));
	free((void *)(node));
}
