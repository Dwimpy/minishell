/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_list_create.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 16:25:57 by arobu             #+#    #+#             */
/*   Updated: 2023/04/25 16:26:10 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quote_list.h"
#include "fsm.h"
#include "ft_printf.h"
#include <stdio.h>

t_quotelist	*new_quotelist(void)
{
	t_quotelist	*qlist;

	qlist = (t_quotelist *)malloc(sizeof(t_quotelist));
	if (!qlist)
		return (NULL);
	qlist->first = NULL;
	qlist->last = NULL;
	qlist->count = 0;
	return (qlist);
}

t_quote	*new_quote_elem(char *value, t_quote_type type)
{
	t_quote	*quote;

	quote = (t_quote *)malloc(sizeof(t_quote));
	quote->value = value;
	quote->type = type;
	quote->next = NULL;
	return (quote);
}
