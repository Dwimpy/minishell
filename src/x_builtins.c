/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   x_builtins.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 15:44:49 by tkilling          #+#    #+#             */
/*   Updated: 2023/04/17 01:58:04 by arobu            ###   ########.fr       */
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

int	ft_unset(char **str_arr, t_input *input)
{
	size_t	i;
	size_t	j;
	void	*str;

	if (str_arr[1] == NULL)
	{
		write(2, "unset: not enough arguments\n", 29);
		return (1);
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
			return (1);
		}
		hashmap_remove(input->hashmap, str_arr[i++]);
	}
	return (0);
}

int	ft_export(char **str_arr, t_input *input)
{
	size_t	i;
	size_t	j;
	char	*str;
	
	str = NULL;
	i = 1;
	if (str_arr[1] == NULL)
		return (ft_env(str_arr, input));
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
				return (1);
			}
			else
			{
				free(hashmap_put_new(input->hashmap, str_arr[i], str));
			}
			i++;
		}
		return (0);
	}
}

int	ft_env(char **str_arr, t_input *input)
{
	char	**arr;
	size_t	i;

	i = 0;
	arr = hashmap_tochar(input->hashmap);
	if (!(char *)hashmap_get(input->hashmap, "PATH"))
	{
		ft_putstr_fd("minishell: env: no such file or directory\n", 2);
		return (1);
	}
	if (str_arr[1] && !ft_memcmp(str_arr[1], "-i", 3))
	{
		if (str_arr[2] && (!ft_memcmp(str_arr[2], "./", 2) || !ft_memcmp(str_arr[2], "../", 3)))
			return (ft_executable_no_env(&str_arr[2], input));
		return (1);
	}
	while (arr[i] != NULL)
	{
		printf("%s\n", arr[i]);
		free(arr[i]);
		i++;
	}
	free(arr);
	return (0);
}


int	ft_echo(char **str_arr, int fd)
{
	int		n_flag;
	size_t	i;
	size_t	j;

	i = 1;
	n_flag = 0;

	while (str_arr[i] != NULL && !ft_memcmp(str_arr[i], "-n", 2))
	{
		j = 1;
		while (str_arr[i][j] == 'n')
			j++;
		if (str_arr[i][j] == '\0')
		{
			i++;
			n_flag = 1;
		}
		else
			break ;
	}
	while (str_arr[i++] != NULL)
	{
		write(fd, str_arr[i - 1], ft_strlen(str_arr[i - 1]));
		if (str_arr[i] != NULL)
			write(fd, " ", 1);
	}
	if (!n_flag)
		write(fd, "\n", 1);
	return (0);
}

int	ft_exit(char **str_arr, t_input *input)
{
	unsigned char	c;
	size_t			i;
	char			*trim;
	i = 0;
	c = 0;

	trim = NULL;
	if (str_arr[1] != NULL)
	{
		if (str_arr[1][0] == '"')
			trim = ft_strtrim(str_arr[1], "\"");
		else if (str_arr[1][0] == '\'')
			trim = ft_strtrim(str_arr[1], "'");
		else
			trim = ft_strdup(str_arr[1]);
		if (str_arr[2] != NULL)
		{
			ft_putstr_fd("minishell: exit: too many arguments\n" ,2);
			return (1);
		}
		if (trim[0] == '-' || trim[0] == '+')
			i++;
		while (trim[i] != '\0')
		{

			if (!(ft_isdigit(trim[i++])))
			{
				ft_putstr_fd("minishell: exit: ", 2);
				ft_putstr_fd(trim, 2);
				ft_putstr_fd(": ", 2);
				ft_putstr_fd("numeric argument required\n", 2);
				exit(c);
			}	
		}
			c = ft_atoi(trim);
			free(trim);
	}
	free (input->lexer.input);
	clear_history();
	free_token_list(input->tokens);
	free(input->tokens);
	ast_del_node(input->root);
	hashmap_free(&input->hashmap);
	hashmap_free(&input->special_sym);
	// system("leaks minishell");
	exit (c);
}

int	ft_cd(char **str_arr, t_input *input)
{
	char	**arr;
	char	*str;
	char	*path;
	char	*old;

	if (str_arr[1] == NULL || !(ft_memcmp("~", str_arr[1], 2)))
	{
		str = (char *)hashmap_get(input->hashmap, "HOME");
		if (!str)
		{
			str = getcwd(NULL, 0);
		}
		old = hashmap_put(input->hashmap, "PWD", ft_strdup(str));
		free(hashmap_put(input->hashmap, "OLDPWD", old));
		if (str)
		{
			if (chdir(str) == -1)
			{
				ft_putstr_fd("cd: " ,2);
				ft_putstr_fd(strerror(errno), 2);
				ft_putstr_fd(": ", 2);
				ft_putstr_fd(str, 2);
				ft_putstr_fd("\n", 2);
				return (1);
			}
		}
	}
	else
	{
		if (!ft_strncmp(str_arr[1], "-", 2))
		{
			if (!(char *)hashmap_get(input->hashmap, "OLDPWD"))
			{
				ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2);
				return (1);
			}
			else if (chdir((char *)hashmap_get(input->hashmap, "OLDPWD")) == -1)
			{
				ft_putstr_fd("minishell: cd: ", 2);
				ft_putstr_fd(str_arr[1], 2);
				ft_putendl_fd(" Not a directory", 2);
				return (1);
			}
		}
		else if (chdir(str_arr[1]) == -1)
		{
			ft_putstr_fd("minishell: cd: ", 2);
			ft_putstr_fd(str_arr[1], 2);
			ft_putendl_fd(" Not a directory", 2);
			return (1);
		}
		path = getcwd(NULL, 0);
		old = hashmap_put(input->hashmap, "PWD", ft_strdup(path));
		free(hashmap_put(input->hashmap, "OLDPWD", old));
		free(path);
	}
	return (0);
}

int	ft_pwd(char **str_arr)
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
		return (1);
	}
	return (0);
}