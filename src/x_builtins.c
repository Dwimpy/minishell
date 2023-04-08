/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   x_builtins.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkilling <tkilling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 15:44:49 by tkilling          #+#    #+#             */
/*   Updated: 2023/04/07 16:03:23 by tkilling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "x_execution.h"
#include <string.h>
#include <errno.h>


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

void	ft_unset(char **str_arr, t_input *input)
{
	size_t	i;
	size_t	j;
	void	*str;

	if (str_arr[1] == NULL)
	{
		write(2, "unset: not enough arguments\n", 29);
	}
	i = 1;
	while (str_arr[i] != NULL)
	{
		j = 0;
		while (str_arr[i][j] != '\0' && (ft_isalnum(str_arr[i][j]) || str_arr[i][j] == '_'))
			j++;
		if (str_arr[i][j] != '\0')
		{
			ft_putstr_fd("unset: ", 2);
			ft_putstr_fd(str_arr[i], 2);
			ft_putstr_fd(" invalid parameter name", 2);
			return ;
		}
		hashmap_remove(input->hashmap, str_arr[i++]);
	}
}

void	ft_export(char **str_arr, t_input *input)
{
	size_t	i;
	size_t	j;
	char	*str;
	
	str = NULL;
	i = 1;
	if (str_arr[1] == NULL)
		ft_env(str_arr, input);
	else
	{
		while (str_arr[i] != NULL)
		{
			j = 0;
			while (str_arr[i][j] != '\0' && (ft_isalnum(str_arr[i][j]) || str_arr[i][j] == '_'))
				j++;
			if (str_arr[i][j] == '=')
			{
				str_arr[i][j] = '\0';
				str = &(str_arr[i][j + 1]);
			}
			else
				str = &(str_arr[i][j]);
			if (str_arr[i][j] != '\0')
			{
				ft_putstr_fd("export: not valid in this context: ", 2);
				ft_putstr_fd(str_arr[i], 2);
			}
			else
			{
				free(hashmap_put_new(input->hashmap, str_arr[i], str));
			}
			i++;
		}
	}
}

void	ft_env(char **str_arr, t_input *input)
{
	char	**arr;
	size_t	i;

	i = 0;
	arr = hashmap_tochar(input->hashmap);
	while (arr[i] != NULL)
	{
		printf("%s\n", arr[i]);
		free(arr[i]);
		i++;
	}
	free(arr);
}


void	ft_echo(char **str_arr, int fd)
{
	int		n_flag;
	size_t	i;

	i = 1;
	n_flag = 0;
	while (str_arr[i] != NULL && !ft_memcmp(str_arr[i], "-n", 3))
	{
		i++;
		n_flag = 1;
	}
	while (str_arr[i++] != NULL)
	{
		write(fd, str_arr[i - 1], ft_strlen(str_arr[i - 1]));
		if (str_arr[i] != NULL)
			write(fd, " ", 1);
	}
	if (!n_flag)
		write(fd, "\n", 1);
}

void	ft_exit(char **str_arr, t_input *input)
{
	if (str_arr[1] != NULL)
	{
		printf("exit: too many arguments");
		return ;
	}
	free (input->lexer.input);
	clear_history();
	free_token_list(input->tokens);
	free(input->tokens);
	ast_del_node(input->root);
	hashmap_free(&input->hashmap);
	system("leaks minishell");
	exit (0);
}

void	ft_cd(char **str_arr, t_input *input)
{
	char	**arr;
	char	*str;
	char	*path;

	if (str_arr[1] == NULL || !(ft_memcmp("~", str_arr[1], 2)))
	{
		str = (char *)hashmap_get(input->hashmap, "HOME");
		free(hashmap_put(input->hashmap, "PWD", ft_strdup(str)));
		if (str)
			if (chdir(str) == -1)
			{
				ft_putstr_fd("cd: " ,2);
				ft_putstr_fd(strerror(errno), 2);
				ft_putstr_fd(": ", 2);
				ft_putstr_fd(str, 2);
				ft_putstr_fd("\n", 2);
			}
	}
	else
	{
		if (chdir(str_arr[1]) == -1)
			printf("Error\n");
		path = getcwd(NULL, 0);
		free(hashmap_put(input->hashmap, "PWD", ft_strdup(path)));
		free(path);
	}
}

void	ft_pwd(char **str_arr)
{
	char	*path;

	if (str_arr[1] == NULL)
	{
		path = getcwd(NULL, 0);
		printf("%s\n", path);
		free(path);
	}
	else
	{
		write(2, "pwd: too many arguments\n", 25);
	}
}