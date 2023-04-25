/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expander.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/16 16:51:47 by arobu             #+#    #+#             */
/*   Updated: 2023/04/25 17:28:50 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

char	*get_env_vars(t_arglist *list, t_input *input)
{
	t_arg	*arg;
	char	*new;
	size_t	len;

	arg = list->first;
	if (!arg)
	{
		free_args(list);
		return (NULL);
	}
	len = count_len(arg, input);
	new = (char *)ft_calloc(len + 1, sizeof(char));
	if (!new)
		return (NULL);
	arg = list->first;
	while (arg)
	{
		get_env_var_loop(arg, new, input);
		arg = arg->next;
	}
	free_args(list);
	return (new);
}

void	get_env_var_loop(t_arg *arg, char *new, t_input *input)
{
	if (arg->type == ESCAPED)
		ft_strcat(new, arg->value);
	else if (arg->type == EXPAND)
		do_arg_expand(new, arg, input);
	else if (arg->type == NON_EXPAND)
		do_arg_non_expand(new, arg);
	if (arg->next == NULL && arg->value && \
		arg->expand_type == 0 && arg->type == EXPAND && \
			!ft_memcmp(arg->value, "$", 2))
		ft_strcat(new, "$");
}

void	do_arg_non_expand(char *new, t_arg *arg)
{
	if (arg->value)
		ft_strcat(new, arg->value);
	else
		ft_strcat(new, "");
}

void	do_arg_expand(char *new, t_arg *arg, t_input *input)
{
	char	*entry;

	if (arg->value && !ft_strncmp(arg->value, "$?", 3))
		entry = (char *)hashmap_get(input->special_sym, "EXITSTATUS");
	else
		entry = (char *)hashmap_get(input->hashmap, &arg->value[1]);
	if (entry)
		ft_strcat(new, entry);
	else if (arg->expand_type == 0 && !ft_strncmp(arg->value, "$", 2))
		ft_strcat(new, "");
	else if (arg->expand_type == 1 && !ft_strncmp(arg->value, "$", 2))
		ft_strcat(new, arg->value);
	else
		ft_strcat(new, "");
}

int	count_len_helper(t_arg *arg, size_t *len, t_input *input)
{
	char	*entry;

	entry = NULL;
	if (arg->type == EXPAND)
	{
		entry = (char *)hashmap_get(input->hashmap, &arg->value[1]);
		if (entry)
			*len += ft_strlen(entry);
		else
			*len += 0;
	}
	else if (arg->type == ESCAPED)
		*len += 1;
	else
	{
		if (arg->value)
			*len += ft_strlen(arg->value);
		else
			*len += 0;
	}
	return (*len);
}
