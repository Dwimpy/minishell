/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   atom.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 01:03:29 by arobu             #+#    #+#             */
/*   Updated: 2023/03/15 01:36:32 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "atom.h"
#include "ft_printf.h"
#include <stdio.h>
t_atom	*atom_new(const void *const bytes, \
				const size_t len, const uint32_t hash)
{
	t_atom	*atom;

	atom = (t_atom *)malloc(sizeof(t_atom));
	if (bytes == NULL)
		return (NULL);
	if (SIZE_MAX - SIZE_MAX / len < 0)
		return (NULL);
	atom->bytes = (void *)malloc(len + 1);
	if (atom->bytes == NULL)
		return (NULL);
	ft_memcpy(atom->bytes, bytes, len);
	atom->len = len;
	atom->hash = hash;
	atom->next = NULL;
	return (atom);
}

uint32_t	atom_hash(const void *const bytes, const size_t len)
{
	size_t			i;
	uint32_t		hash;
	const uint8_t	*key = bytes;

	i = 0;
	while (i < len)
	{
		hash += key[i];
		hash += hash << 10;
		hash ^= hash >> 6;
		i++;
	}
	hash += hash << 3;
	hash ^= hash >> 11;
	hash += hash << 15;
}
