/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_list_helpers.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 16:23:51 by arobu             #+#    #+#             */
/*   Updated: 2023/04/25 16:24:57 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quote_list.h"
#include "fsm.h"
#include "ft_printf.h"
#include <stdio.h>

int	end_of_input(char *value, t_index *index, t_fsm *fsm)
{
	if (value[index->i] == '\0')
	{
		fsm->input_state = INPUT_COMPLETE;
		return (1);
	}
	return (0);
}

void	add_quote_element(t_quotelist *list, t_quote *quote)
{
	if (!quote)
		return ;
	if (!list->first)
	{
		list->first = quote;
		list->last = quote;
	}
	else
	{
		list->last->next = quote;
		list->last = quote;
	}
	list->count++;
}

void	print_quotelist(t_quotelist *list)
{
	t_quote	*arg;

	if (!list)
		return ;
	arg = list->first;
	printf("Args: ");
	while (arg)
	{
		printf("%s | ", arg->value);
		arg = arg->next;
	}
	printf("\n");
}

void	free_quotelist(t_quotelist *list)
{
	t_quote	*quote;

	if (!list)
		return ;
	quote = list->first;
	while (quote)
	{
		list->first = list->first->next;
		free(quote->value);
		free(quote);
		quote = list->first;
	}
	free(list);
}
