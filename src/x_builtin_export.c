/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   x_builtin_export.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkilling <tkilling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 17:26:33 by tkilling          #+#    #+#             */
/*   Updated: 2023/04/24 17:59:20 by tkilling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "x_execution.h"
#include <string.h>
#include <errno.h>

static int	check_valid_env_ident(char *str_arr, t_input *input, int *status);
void		handle_whitespaces(char *str);
static void	*hashmap_put_new(t_hashmap *hashmap, const void *key, void *value);
void		ft_execute_export(char **str_arr, t_input *input,
				char **str, size_t i);

int	ft_export(char **str_arr, t_input *input)
{
	size_t	i;
	char	*str;
	int		status;

	str = NULL;
	i = 1;
	status = 0;
	if (str_arr[1] == NULL)
		return (ft_env(str_arr, input));
	while (str_arr[i] != NULL)
	{
		if (!check_valid_env_ident(str_arr[i], input, &status))
			ft_execute_export(str_arr, input, &str, i);
		i++;
	}
	return (status);
}

void	ft_execute_export(char **str_arr, t_input *input, char **str, size_t i)
{
	if (ft_isalpha(str_arr[i][0]) && ft_strchr(str_arr[i], '+')
		&& (ft_strchr(str_arr[i], '+') + 1)[0] == '=')
	{
		*str = ft_strchr(str_arr[i], '=') + 1;
		ft_strchr(str_arr[i], '+')[0] = '\0';
		free(hashmap_put_new(input->hashmap, str_arr[i], *str));
	}
	else if (ft_strchr(str_arr[i], '=') && ft_isalpha(str_arr[i][0]))
	{
		*str = ft_strchr(str_arr[i], '=') + 1;
		ft_strchr(str_arr[i], '=')[0] = '\0';
		handle_whitespaces(*str);
		free(hashmap_put_new(input->hashmap, str_arr[i], *str));
	}
}

int	check_valid_env_ident(char *str_arr, t_input *input, int *status)
{
	int		j;
	char	*copy;

	copy = strdup(str_arr);
	if (!ft_isalpha(copy[0]) && copy[0] != '_')
	{
		ft_putstr_fd("minishell: export: `", 2);
		ft_putstr_fd(copy, 2);
		ft_putendl_fd("': not a valid identifier", 2);
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
			ft_putstr_fd("minishell: export: `", 2);
			ft_putstr_fd(copy, 2);
			ft_putendl_fd("': not a valid identifier", 2);
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

static void	*hashmap_put_new(t_hashmap *hashmap, const void *key, void *value)
{
	t_entry	*entry;
	int		index;
	void	*prev;

	if (!hashmap && !hashmap->table && !key)
		return (NULL);
	index = hashmap->hash(key) % hashmap->size;
	entry = hashmap->table[index];
	while (entry)
	{
		if (hashmap->compare(key, entry->key))
			break ;
		entry = entry->next;
	}
	if (!entry)
	{
		entry = (t_entry *)malloc(sizeof(t_entry));
		entry->key = ft_strdup((char *)key);
		entry->value = ft_strdup((char *)value);
		entry->next = hashmap->table[index];
		hashmap->table[index] = entry;
		hashmap->length++;
		prev = NULL;
	}
	else
	{
		prev = entry->value;
		entry->value = ft_strdup((char *)value);
	}
	return (prev);
}

void	handle_whitespaces(char *str)
{
	int		i;
	int		j;
	char	*str_cpy;

	i = 0;
	j = 0;
	str_cpy = ft_strdup(str);
	while (str[j])
	{
		if (ft_isspace3(str_cpy[i]))
			str[j++] = str_cpy[i++];
		while (ft_isspace3(str_cpy[i]))
			i++;
		if (str_cpy[i] == '\0')
		{
			str[j] = '\0';
			break ;
		}
		str[j] = str_cpy[i];
		i++;
		j++;
	}
}
