/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expander.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/16 16:51:47 by arobu             #+#    #+#             */
/*   Updated: 2023/04/16 18:08:34 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

char	*get_env_vars(t_arglist *list, t_input *input)
{
	t_arg	*arg;
	char	*entry;
	char	*new;
	size_t	len;

	arg = list->first;
	if (!arg)
		return (NULL);
	len = 0;
	while (arg)
	{
		if (arg->type == EXPAND)
		{
			entry = (char *)hashmap_get(input->hashmap, arg->value);
			if (entry)
				len += ft_strlen(entry);
			else
				len += 0;
		}
		if (arg->value)
			len += ft_strlen(arg->value);
		else
			len += 0;
		arg = arg->next;
	}
	new = (char *)ft_calloc(len + 1, sizeof(char));
	if (!new)
		return (NULL);
	arg = list->first;
	while (arg)
	{
		if (arg->type == EXPAND)
		{
			entry = (char *)hashmap_get(input->hashmap, arg->value);
			if (entry)
				ft_strcat(new, entry);
			else
				ft_strcat(new, "");
		}
		else if (arg->type == NON_EXPAND)
		{
		if (arg->value)
			ft_strcat(new, arg->value);
		else
			ft_strcat(new, "");
		}
		arg = arg->next;
	}
	free_args(list);
	return (new);
}

void	expand_env_vars(char **args, t_input *input)
{
	char	*prev;

	prev = NULL;
	while (args && *args)
	{
		prev = *args;
		*args = get_env_vars(expand_vars(*args), input);
		if (!*args)
		{
			*args = prev;
			break ;
		}
		args++;
		free(prev);
	}
	return ;
}
