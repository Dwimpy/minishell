/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   x_builtin_exit.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 18:06:17 by tkilling          #+#    #+#             */
/*   Updated: 2023/04/26 13:58:46 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "x_execution.h"
#include <string.h>
#include <errno.h>

void	ft_free(t_input *input);
void	ft_trim(char **trim, char **str_arr);
void	ft_if_numeric(char *trim, size_t *i);

int	ft_exit(char **str_arr, t_input *input)
{
	unsigned char	c;
	char			*trim;

	c = 0;
	trim = NULL;
	if (str_arr[1] != NULL)
	{
		ft_trim(&trim, str_arr);
		if (str_arr[2] != NULL)
		{
			ft_putstr_fd("minishell: exit: too many arguments\n", 2);
			return (1);
		}
		c = ft_atoi(trim);
		free(trim);
	}
	ft_free(input);
	exit (c);
}

void	ft_trim(char **trim, char **str_arr)
{
	size_t			i;

	i = 0;
	if (str_arr[1][0] == '"')
		*trim = ft_strtrim(str_arr[1], "\"");
	else if (str_arr[1][0] == '\'')
		*trim = ft_strtrim(str_arr[1], "'");
	else
		*trim = ft_strdup(str_arr[1]);
	if ((*trim)[0] == '-' || (*trim)[0] == '+')
		i++;
	if ((*trim)[0] == '\0')
		ft_if_numeric((*trim), &i);
	while ((*trim)[i] != '\0')
		ft_if_numeric(*trim, &i);
}

void	ft_if_numeric(char *trim, size_t *i)
{
	if (!(ft_isdigit(trim[(*i)++])))
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(trim, 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd("numeric argument required\n", 2);
		exit(255);
	}
}

void	ft_free(t_input *input)
{
	t_ast_node		*root;

	free (input->lexer.input);
	clear_history();
	free_token_list(input->tokens);
	free(input->tokens);
	root = input->root;
	while (root->left != NULL)
		root = root->left;
	ft_change_tree_back(root);
	ast_del_node(input->root);
	hashmap_free(&input->hashmap);
	hashmap_free(&input->special_sym);
	free_heredoc_list(input->heredoc_files);
	free(input->heredoc_files);
}
