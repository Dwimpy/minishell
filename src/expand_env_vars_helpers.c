/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_env_vars_helpers.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 17:26:11 by arobu             #+#    #+#             */
/*   Updated: 2023/04/25 17:29:21 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	expand_env_vars(char **args, t_input *input)
{
	char		*prev;

	prev = NULL;
	if (args && *args && !ft_strncmp(*args, "~", 2))
	{
		prev = *args;
		*args = ft_strdup((char *)hashmap_get(input->special_sym, "TILDE"));
		free(prev);
	}
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

int	count_len(t_arg *arg, t_input *input)
{
	size_t	len;

	len = 0;
	while (arg)
	{
		count_len_helper(arg, &len, input);
		arg = arg->next;
	}
	return (len);
}
