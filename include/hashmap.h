/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashmap.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 15:06:04 by arobu             #+#    #+#             */
/*   Updated: 2023/04/25 00:20:30 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HASHMAP_H
# define HASHMAP_H

# include <stdlib.h>
# include <stdint.h>
# include <limits.h>
# include "ft_printf.h"

typedef struct s_entry	t_entry;

typedef struct s_entry
{
	const void	*key;
	void		*value;
	t_entry		*next;
}				t_entry;

typedef struct s_hashmap
{
	t_entry		**table;
	int			size;
	int			length;
	int			(*compare)(const void *x, const void *y);
	uint32_t	(*hash)(const void *key);
}				t_hashmap;

t_hashmap	*hashmap_new(int size);
int			hashmap_length(t_hashmap *hashmap);
void		*hashmap_remove(t_hashmap *hashmap, const void *key);
void		*hashmap_put(t_hashmap *hashmap, \
					const void *key, void *value);
void		**hashmap_toarray(t_hashmap	*hashmap, void *end);
int			compare_keys(const void *x, const void *y);
void		*hashmap_get(t_hashmap *hashmap, const void *key);
uint32_t	table_hash(const void *key);
char		**hashmap_tochar(t_hashmap *hashmap);
void		hashmap_free(t_hashmap **hashmap);
t_entry		*create_entry(char *key, char *value);

#endif
