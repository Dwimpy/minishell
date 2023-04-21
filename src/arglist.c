/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arglist.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/19 14:17:44 by arobu             #+#    #+#             */
/*   Updated: 2023/04/21 19:31:35 by arobu            ###   ########.fr       */
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

void	print_args(t_arglist *arglist)
{
	t_arg	*arg;

	if (!arglist)
		return ;
	arg = arglist->first;
	printf("Args: \n");
	while (arg)
	{
		// if (arg->type == EXPAND)
		printf("%s |", arg->value);
		arg = arg->next;
	}
	printf("\n");
}

t_arg	*create_expand_arg(char *value, unsigned int start, size_t len, t_arg_type type)
{
	t_arg	*arg;

	arg = (t_arg *)malloc(sizeof(t_arg));
	if (!arg)
		return (NULL);
	arg->value = ft_substr(value, start, len);
	arg->type = type;
	// printf("IN FUNC: %s\n", arg->value);
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

t_arg		*create_heredoc_file(char *value)
{
	t_arg	*arg;

	arg = (t_arg *)malloc(sizeof(t_arg));
	if (!arg)
		return (NULL);
	arg->value = ft_strdup(value);
	// printf("IN FUNC: %s\n", arg->value);
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

void	remove_node(t_arglist *arglist, t_arg *remove)
{
	t_arg	*arg;
	t_arg	*prev;

	arg = arglist->first;
	if (remove == arglist->first)
	{
		arglist ->first = arglist->first->next;
		if (remove->value)
			free(remove->value);
		free(remove);
		remove = NULL;
		return ;
	}
	else if (remove == arglist->last)
	{
		while (arg->next == NULL)
			arg = arg->next;
		if (remove->value)
			free(remove->value);
		free(remove);
		remove = NULL;
		arg->next = NULL;
		arglist->last = arg;
		return ;
	}
	else
	{
		while (arg != remove)
		{
			prev = arg;
			arg = arg->next;
		}
	prev = arg->next;
		if (arg->value)
			free(arg->value);
		free(arg);
		return ;	
	}
}
