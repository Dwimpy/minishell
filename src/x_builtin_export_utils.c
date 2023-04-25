/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   x_builtin_export_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkilling <tkilling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 19:54:26 by tkilling          #+#    #+#             */
/*   Updated: 2023/04/24 20:05:56 by tkilling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "x_execution.h"
#include <string.h>
#include <errno.h>

void	ft_identifier_error(char *copy);
int		ft_check_str(t_input *input, int *status, char *copy);

int	check_valid_env_ident(char *str_arr, t_input *input, int *status)
{
	char	*copy;

	copy = strdup(str_arr);
	if (!ft_isalpha(copy[0]) && copy[0] != '_')
	{
		ft_identifier_error(copy);
		free(hashmap_put(input->hashmap, "EXITSTATUS", ft_itoa(*status)));
		if (copy[0] == '-')
			*status = 2;
		else
			*status = 1;
		free(copy);
		return (*status);
	}
	if (ft_strchr(copy, '='))
		ft_strchr(copy, '=')[1] = '\0';
	return (ft_check_str(input, status, copy));
}

int	ft_check_str(t_input *input, int *status, char *copy)
{
	int		j;

	j = 0;
	while (copy[j])
	{
		if (ft_strchr("@%^*#-!&~.{}+", copy[j]))
		{
			if (copy[j] == '+' && copy[j + 1] == '=')
			{
				j++;
				continue ;
			}
			ft_identifier_error(copy);
			if (copy[0] == '-')
				*status = 2;
			else
				*status = 1;
			free(hashmap_put(input->hashmap, "EXITSTATUS", ft_itoa(*status)));
			free(copy);
			return (*status);
		}
		j++;
	}
	return (0);
}

void	ft_identifier_error(char *copy)
{
	ft_putstr_fd("minishell: export: `", 2);
	ft_putstr_fd(copy, 2);
	ft_putendl_fd("': not a valid identifier", 2);
}
