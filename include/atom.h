/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   atom.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 20:37:32 by arobu             #+#    #+#             */
/*   Updated: 2023/03/15 01:18:35 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ATOM_H
# define ATOM_H
# include <stdlib.h>
# include <stdint.h>

typedef struct s_atom	t_atom;

typedef struct s_atom
{
	int			len;
	uint32_t	hash;
	void		*bytes;
	t_atom		*next;
}				t_atom;

t_atom		*atom_new(const void *const bytes, \
				const size_t len, const uint32_t hash);
uint32_t	atom_hash(const void *const bytes, const size_t len);

#endif
