/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arglist.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/19 14:15:28 by arobu             #+#    #+#             */
/*   Updated: 2023/04/12 22:37:40 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ARGLIST_H
# define ARGLIST_H

# include <stdlib.h>
# include "token.h"

typedef struct s_arg	t_arg;

typedef enum e_arg_type
{
	NORMAL,
	QUOTED_ARG
}				t_arg_type;

typedef struct s_arg
{
	char		*value;
	t_arg		*next;
	t_arg_type	type;
}				t_arg;

typedef struct t_arglist
{
	t_arg			*first;
	t_arg			*last;
	size_t			arg_count;
}					t_arglist;

t_arglist		*new_arglist(void);
void			new_argument(t_arglist	*arglist, t_arg *arg);
t_arg			*copy_arg(t_arg *arg);
t_arg			*create_arg(t_token *token, t_arg_type type);
char			*get_token_value(t_token *token);
void			print_args(t_arglist *arglist);
void			free_args(t_arglist *arglists);


#endif
