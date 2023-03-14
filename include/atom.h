/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   atom.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 20:37:32 by arobu             #+#    #+#             */
/*   Updated: 2023/03/14 22:27:41 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ATOM_H
# define ATOM_H

typedef struct s_atom	t_atom;

typedef struct s_atom
{
	int			len;
	char		*hash;
	void		*bytes;
	t_atom		*next;
}				t_atom;



#endif
