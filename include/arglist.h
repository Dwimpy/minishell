/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arglist.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/19 14:15:28 by arobu             #+#    #+#             */
/*   Updated: 2023/03/19 14:30:08 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ARGLIST_H
# define ARGLIST_H

#include <stdlib.h>

typedef struct s_arg	t_arg;

typedef struct s_arg
{
	char		*value;
	t_arg		*next;
}				t_arg;

typedef struct t_arglist
{
	t_arg			*first;
	t_arg			*last;
	size_t			arg_count;
}					t_arglist;

t_arglist		*new_arglist(void);
void			new_argument(t_arglist	*arglist, t_arg *arg);
t_arg			*create_arg(char *value);
void			print_args(t_arglist *arglist);
void			free_args(t_arglist *arglists);


#endif
