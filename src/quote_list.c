/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 19:36:15 by arobu             #+#    #+#             */
/*   Updated: 2023/04/14 14:03:34 by arobu            ###   ########.fr       */
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

t_quotelist	*create_list(char *value)
{
	int			i;
	int			start;
	t_fsm		fsm;
	t_quotelist	*list;

	i = 0;
	if (!value)
		return (NULL);
	list = new_quotelist();
	fsm.input_state = N_INPUT;
	while (fsm.input_state != INPUT_COMPLETE)
	{
		if (value[i] == '\0')
		{
			fsm.input_state = INPUT_COMPLETE;
			break ;
		}
		if (fsm.input_state == N_INPUT)
		{
			if (value[i] == '\'')
				fsm.input_state = IN_SQUOTE;
			else if (value[i] == '\"')
				fsm.input_state = IN_DQUOTE;
			else
				fsm.input_state = REGULAR;
			start = i;
		}
		else if (fsm.input_state == IN_SQUOTE)
		{
			if (value[i] == '\'')
			{
				fsm.input_state = N_INPUT;
				add_quote_element(list, new_quote_elem(ft_substr(value, start, i - start + 1), QUOTE_SQUOTE));
			}
		}
		else if (fsm.input_state == IN_DQUOTE)
		{
			if (value[i] == '\"')
			{
				fsm.input_state = N_INPUT;
				add_quote_element(list, new_quote_elem(ft_substr(value, start, i - start + 1), QUOTE_DQUOTE));
			}
		}
		else if (fsm.input_state == REGULAR)
		{
			if (value[i] == '\'')
			{
				fsm.input_state = IN_SQUOTE;
				add_quote_element(list, new_quote_elem(ft_substr(value, start, i - start), QUOTE_REGULAR));
				start = i;
			}
			else if (value[i] == '\"')
			{
				fsm.input_state = IN_DQUOTE;
				add_quote_element(list, new_quote_elem(ft_substr(value, start, i - start), QUOTE_REGULAR));
				start = i;
			}
			else if (value[i + 1] == '\0')
				add_quote_element(list, new_quote_elem(ft_substr(value, start, i - start + 1), QUOTE_REGULAR));
		}
		i++;
	}
	return (list);
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
