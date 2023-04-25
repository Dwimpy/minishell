/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_list.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 19:34:58 by arobu             #+#    #+#             */
/*   Updated: 2023/04/25 17:35:09 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef QUOTE_LIST_H
# define QUOTE_LIST_H

# include <stdlib.h>

typedef struct s_quote	t_quote;

typedef enum t_quote_type
{
	QUOTE_REGULAR,
	QUOTE_SQUOTE,
	QUOTE_DQUOTE
}				t_quote_type;

typedef struct s_quote
{
	char			*value;
	t_quote			*next;
	t_quote_type	type;
}				t_quote;

typedef struct s_quotelist
{
	t_quote			*first;
	t_quote			*last;
	size_t			count;
}					t_quotelist;

t_quotelist		*new_quotelist(void);
t_quote			*new_quote_elem(char *value, t_quote_type type);
t_quotelist		*create_list(char *value);
void			print_quotelist(t_quotelist *list);
void			add_quote_element(t_quotelist *list, t_quote *quote);
void			free_quotelist(t_quotelist *list);
#endif
