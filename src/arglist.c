/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arglist.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/19 14:17:44 by arobu             #+#    #+#             */
/*   Updated: 2023/03/23 17:20:33 by arobu            ###   ########.fr       */
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

t_arg	*create_arg(t_token *token)
{
	t_arg	*arg;

	arg = (t_arg *)malloc(sizeof(t_arg));
	if (!arg)
		return (NULL);
	arg->value = ft_strdup(get_token_value(token));
	arg->next = NULL;
	return (arg);
}

char	*get_token_value(t_token *token)
{
	if (token->type == TOKEN_WORD)
		return (token->value.word.value);
	if (token->type == TOKEN_SQUOTE)
		return (token->value.squote.value);
	if (token->type == TOKEN_DQUOTE)
		return (token->value.dquote.value);
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

void	print_args(t_arglist *arglist)
{
	t_arg	*arg;

	if (!arglist)
		return ;
	arg = arglist->first;
	printf("Args: ");
	while (arg)
	{
		printf("%s | ", arg->value);
		arg = arg->next;
	}
	printf("\n");
}

void	free_args(t_arglist *arglist)
{
	t_arg	*arg;

	if (!arglist)
		return ;
	arg = arglist->first;
	while (arg)
	{
		arglist->first = arglist->first->next;
		free(arg->value);
		free(arg);
		arg = arglist->first;
	}
	free(arglist);
}
