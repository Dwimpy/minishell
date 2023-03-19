/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arglist.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/19 14:17:44 by arobu             #+#    #+#             */
/*   Updated: 2023/03/19 14:49:19 by arobu            ###   ########.fr       */
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

t_arg	*create_arg(char *value)
{
	t_arg	*arg;

	arg = (t_arg *)malloc(sizeof(t_arg));
	if (!arg)
		return (NULL);
	arg->value = ft_strdup(value);
	arg->next = NULL;
	return (arg);
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
}
