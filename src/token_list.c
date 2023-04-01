/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 12:28:04 by arobu             #+#    #+#             */
/*   Updated: 2023/04/01 14:29:37 by arobu            ###   ########.fr       */
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

t_token	*peek_token(t_token_list *tokens, int look_ahead_times)
{
	t_token	*token;
	int		i;

	i = 0;
	while (token && i < look_ahead_times)
	{
		token = token->next;
	}
	return (token);
}

t_token	*get_next_token(t_token *token)
{
	(token) = (token)->next;
	return (token);
}

int	get_nargs(t_token *token)
{
	t_token	*curr_token;
	int		count;

	curr_token = token;
	count = 0;
	while (curr_token->type == TOKEN_WORD)
	{
		count++;
		curr_token = curr_token->next;
	}
	return (count);
}

void	consume_token(t_token_list *tokens)
{
	t_token	*token;

	token = (tokens)->first;
	(tokens)->first = (tokens)->first->next;
	free_token(token);
	tokens->num_tokens--;
	return ;
}

t_token	*get_last_token(t_token_list *token_list)
{
	t_token	*token;

	token = token_list->first;
	if (!token)
		return (NULL);
	while (token->next->type != TOKEN_EOF)
		token = token->next;
	return (token);
}
