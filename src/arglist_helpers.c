/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arglist_helpers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 20:09:12 by arobu             #+#    #+#             */
/*   Updated: 2023/04/24 20:09:57 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "arglist.h"
#include "ft_printf.h"
#include <stdio.h>

void	print_args(t_arglist *arglist)
{
	t_arg	*arg;

	if (!arglist)
		return ;
	arg = arglist->first;
	printf("Args: \n");
	while (arg)
	{
		printf("%s |", arg->value);
		arg = arg->next;
	}
	printf("\n");
}

t_arg	*create_expand_arg(char *value, unsigned int start, \
		size_t len, t_arg_type type)
{
	t_arg	*arg;

	arg = (t_arg *)malloc(sizeof(t_arg));
	if (!arg)
		return (NULL);
	arg->value = ft_substr(value, start, len);
	arg->type = type;
	arg->next = NULL;
	return (arg);
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

t_arg	*create_heredoc_file(char *value)
{
	t_arg	*arg;

	arg = (t_arg *)malloc(sizeof(t_arg));
	if (!arg)
		return (NULL);
	arg->value = ft_strdup(value);
	arg->next = NULL;
	return (arg);
}

void	free_heredoc_list(t_arglist *arglist)
{
	t_arg	*arg;

	if (!arglist)
		return ;
	arg = arglist->first;
	while (arg)
	{
		arglist->first = arglist->first->next;
		unlink(arg->value);
		free(arg->value);
		free(arg);
		arg = arglist->first;
	}
}
