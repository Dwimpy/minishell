/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expander.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/16 16:51:47 by arobu             #+#    #+#             */
/*   Updated: 2023/04/17 16:21:36 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

char	*get_env_vars(t_arglist *list, t_input *input)
{
	t_arg	*arg;
	char	*entry;
	char	*new;
	size_t	len;
	t_arg	*empty;

	arg = list->first;
	if (!arg)
	{
		free_args(list);
		return (NULL);
	}
	len = 0;
	while (arg)
	{
		if (arg->type == EXPAND)
		{
			entry = (char *)hashmap_get(input->hashmap, &arg->value[1]);
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
			// printf("%d\t", arg->expand_type);
			// printf("%s\n", arg->value);
			if (arg->value && !ft_strncmp(arg->value, "$?", 3))
				entry = (char *)hashmap_get(input->special_sym, "EXITSTATUS");
			else
				entry = (char *)hashmap_get(input->hashmap, &arg->value[1]);
			if (entry)
				ft_strcat(new, entry);
			else if (arg->expand_type == 0)
				ft_strcat(new, "");
			else if (arg->expand_type == 1)
				ft_strcat(new, arg->value);
		}
		else if (arg->type == NON_EXPAND)
		{
		if (arg->value)
		
			ft_strcat(new, arg->value);
		else
			ft_strcat(new, "");
		}
		if (arg->next == NULL && arg->value && \
			arg->expand_type == 0 && !ft_memcmp(arg->value, "$", 2))
			ft_strcat(new, "$");
		// else if (arg->next != NULL && arg->value[0] == '\0')
		// 	ft_strcat(new, "$");
		arg = arg->next;
	}
	// print_args(list);
	free_args(list);
	return (new);
}

void	expand_env_vars(char **args, t_input *input)
{
	char		*prev;

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
