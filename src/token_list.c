/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 12:28:04 by arobu             #+#    #+#             */
/*   Updated: 2023/04/24 20:13:51 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token_list.h"

t_token_list	*new_token_list(void)
{
	t_token_list	*list;

	list = malloc(sizeof(t_token_list));
	if (!list)
		return (NULL);
	list->first = NULL;
	list->last = NULL;
	list->num_tokens = 0;
	return (list);
}

void	add_token(t_token_list *list, t_token *token)
{
	if (!token)
		return ;
	if (!list->first)
	{
		list->first = token;
		list->last = token;
	}
	else
	{
		list->last->next = token;
		list->last = token;
	}
	list->num_tokens++;
}

void	remove_token(t_token_list *list)
{
	t_token		*tmp;

	if (!list->first)
		return ;
	tmp = list->first;
	list->first = list->first->next;
	free_token(tmp);
	list->num_tokens--;
}

void	free_token_list(t_token_list *list)
{
	t_token		*tmp;

	while (list && list->first)
	{
		tmp = list->first;
		list->first = list->first->next;
		free_token(tmp);
		list->num_tokens--;
	}
}

void	print_tokens(t_token_list *list)
{
	t_token	*token;

	if (!list)
		return ;
	token = list->first;
	while (token)
	{
		print_token_value(token);
		token = token->next;
	}
}
