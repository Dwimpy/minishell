/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashmap_operations.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/18 12:30:16 by arobu             #+#    #+#             */
/*   Updated: 2023/03/18 12:31:08 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hashmap.h"

void	*hashmap_get(t_hashmap *hashmap, const void *key)
{
	t_entry	*entry;
	int		index;

	if (!hashmap && !hashmap->table && !key)
		return (NULL);
	index = hashmap->hash(key) % hashmap->size;
	entry = hashmap->table[index];
	while (entry)
	{
		if (hashmap->compare(key, entry->key) == 0)
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
	while (entry)
	{
		if (hashmap->compare(key, entry->key) == 0)
			break ;
		entry = entry->next;
	}
	if (!entry)
	{
		entry = (t_entry *)malloc(sizeof(t_entry));
		entry->key = key;
		entry->value = value;
		entry->next = hashmap->table[index];
		hashmap->table[index] = entry;
		hashmap->length++;
		prev = NULL;
	}
	else
		prev = entry->value;
	entry->value = value;
	return (prev);
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
		if (hashmap->compare(key, (*entry_ptr)->key) == 0)
		{
			entry = (*entry_ptr);
			value = entry->value;
			(*entry_ptr) = entry->next;
			free(entry);
			hashmap->length--;
			return (value);
		}
		entry_ptr = &(*entry_ptr)->next;
	}
	return (NULL);
}

void	**hashmap_toarray(t_hashmap	*hashmap, void *end)
{
	int		i;
	int		j;
	void	**array;
	t_entry	*entry;

	i = 0;
	j = 0;
	if (!hashmap && !hashmap->table)
		return (NULL);
	array = malloc((2 * hashmap->length + 1) * sizeof(*array));
	i = 0;
	while (i < hashmap->size)
	{
		entry = hashmap->table[i];
		while (entry)
		{
			array[j++] = (void *)entry->key;
			array[j++] = entry->value;
			entry = entry->next;
		}
		i++;
	}
	array[j] = end;
	return (array);
}

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
				free(prev_entry);
			}
			i++;
		}
	}
	free((*hashmap)->table);
	free(*hashmap);
}
