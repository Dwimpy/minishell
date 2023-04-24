/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   x_builtin_exit.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkilling <tkilling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 18:06:17 by tkilling          #+#    #+#             */
/*   Updated: 2023/04/24 18:09:27 by tkilling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "x_execution.h"
#include <string.h>
#include <errno.h>

int	ft_exit(char **str_arr, t_input *input)
{
	unsigned char	c;
	size_t			i;
	char			*trim;
	t_ast_node		*root;

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
		if (trim[0] == '-' || trim[0] == '+')
			i++;
		if (trim[0] == '\0')
		{
			if (!(ft_isdigit(trim[i++])))
			{
				ft_putstr_fd("minishell: exit: ", 2);
				ft_putstr_fd(trim, 2);
				ft_putstr_fd(": ", 2);
				ft_putstr_fd("numeric argument required\n", 2);
				exit(255);
			}
		}
		while (trim[i] != '\0')
		{
			if (!(ft_isdigit(trim[i++])))
			{
				ft_putstr_fd("minishell: exit: ", 2);
				ft_putstr_fd(trim, 2);
				ft_putstr_fd(": ", 2);
				ft_putstr_fd("numeric argument required\n", 2);
				exit(255);
			}
		}
		if (str_arr[2] != NULL)
		{
			ft_putstr_fd("minishell: exit: too many arguments\n", 2);
			return (1);
		}
		c = ft_atoi(trim);
		free(trim);
	}
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
	exit (c);
}
