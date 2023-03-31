/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashmap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 15:21:04 by arobu             #+#    #+#             */
/*   Updated: 2023/03/31 21:20:15 by arobu            ###   ########.fr       */
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

int	compare_keys(const void *x, const void *y)
{
	return (ft_strncmp((char *)x, (char *)y, ft_strlen((char *)x)) == 0);
}

int	hashmap_length(t_hashmap *hashmap)
{
	if (!hashmap && !hashmap->table)
		return (-1);
	return (hashmap->length);
}
