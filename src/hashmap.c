/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashmap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 15:21:04 by arobu             #+#    #+#             */
/*   Updated: 2023/03/18 12:33:25 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hashmap.h"
#include <stdio.h>

t_hashmap	*hashmap_new(int size)
{
	const int	primes[] = {509, 509, 1021, 2053, \
		4093, 8191, 16381, 65521, INT_MAX};
	t_hashmap	*hashmap;
	int			i;

	i = 1;
	hashmap = (t_hashmap *)malloc(sizeof(t_hashmap));
	while (primes[i++] < size)
		;
	hashmap->size = primes[i - 1];
	hashmap->table = (t_entry **)malloc(sizeof(t_entry *) * primes[i - 1]);
	i = 0;
	while (i < hashmap->size)
	{
		hashmap->table[i] = NULL;
		i++;
	}
	hashmap->hash = &table_hash;
	hashmap->compare = &compare_keys;
	hashmap->length = 0;
	return (hashmap);
}

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

int	compare_keys(const void *x, const void *y)
{
	return (x != y);
}

int	hashmap_length(t_hashmap *hashmap)
{
	if (!hashmap && !hashmap->table)
		return (-1);
	return (hashmap->length);
}

uint32_t	table_hash(const void *key)
{
	size_t			i;
	uint32_t		hash;
	const uint8_t	*keyy = key;

	i = 0;
	hash = 0;
	while (keyy && keyy[i])
	{
		hash += keyy[i++];
		hash += hash << 10;
		hash ^= hash >> 6;
	}
	hash += hash << 3;
	hash ^= hash >> 11;
	hash += hash << 15;
	return (hash);
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
