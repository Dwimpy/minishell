/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arglist.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/19 14:17:44 by arobu             #+#    #+#             */
/*   Updated: 2023/04/24 20:09:04 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "arglist.h"
#include "ft_printf.h"
#include <stdio.h>

t_arglist	*new_arglist(void)
{
	t_arglist	*arglist;

	arglist = (t_arglist *)malloc(sizeof(t_arglist));
	if (!arglist)
		return (NULL);
	arglist->first = NULL;
	arglist->last = NULL;
	arglist->arg_count = 0;
	return (arglist);
}

t_arg	*create_arg(t_token *token, t_arg_type type)
{
	t_arg	*arg;

	arg = (t_arg *)malloc(sizeof(t_arg));
	if (!arg)
		return (NULL);
	arg->value = ft_strdup(get_token_value(token));
	arg->type = type;
	arg->next = NULL;
	return (arg);
}

t_arg	*copy_arg(t_arg *arg)
{
	t_arg	*copy;

	copy = (t_arg *)malloc(sizeof(t_arg));
	if (!copy)
		return (NULL);
	copy->value = ft_strdup(arg->value);
	copy->next = NULL;
	return (copy);
}

char	*get_token_value(t_token *token)
{
	if (token->type == TOKEN_WORD)
		return (token->value.word.value);
	if (token->type == TOKEN_QUOTE)
		return (token->value.quote.value);
	if (token->type == TOKEN_ASSIGN_WORD)
		return (token->value.assign_word.value);
	return (NULL);
}

void	new_argument(t_arglist	*arglist, t_arg *arg)
{
	if (!arg)
		return ;
	if (!arglist->first)
	{
		arglist->first = arg;
		arglist->last = arg;
	}
	else
	{
		arglist->last->next = arg;
		arglist->last = arg;
	}
	arglist->arg_count++;
}
