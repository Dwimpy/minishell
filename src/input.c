/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/03 00:42:57 by arobu             #+#    #+#             */
/*   Updated: 2023/04/24 22:24:56 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "environment_handler.h"
#include "quote_list.h"

int	parse_all_input(t_input *input)
{
	size_t	sub_count;

	sub_count = 0;
	if (input->tokens->first->type == TOKEN_QUOTE && \
	input->tokens->num_tokens == 2 && \
		!ft_strncmp(input->tokens->first->value.quote.value, "\"\"", 3))
	{
		ft_putstr_fd("minishell: : command not found\n", 2);
		free(input->lexer.input);
		free_token_list(input->tokens);
		free_heredoc_list(input->heredoc_files);
		free(hashmap_put(input->special_sym, "EXITSTATUS", ft_itoa(127)));
		return (1);
	}
	while (input->tokens->first->type != TOKEN_EOF)
		parse_input(&input->root, input->tokens, input, &sub_count);
	return (0);
}

t_hashmap	*create_special_table(void)
{
	int			i;
	int			count;
	char		*home;
	char		*path;
	t_hashmap	*hashmap;

	hashmap = hashmap_new(100);
	path = getcwd(NULL, 0);
	i = 0;
	count = 0;
	while (path && path[i])
	{
		if (path[i] == '/')
			count++;
		if (count == 3)
			break ;
		i++;
	}
	home = ft_substr(path, 0, &path[i] - path);
	hashmap_put(hashmap, "TILDE", home);
	hashmap_put(hashmap, "EXITSTATUS", ft_strdup("0"));
	free(path);
	return (hashmap);
}

void	init_input(t_input	*input, char **envp)
{
	input->tokens = new_token_list();
	input->heredoc_files = new_arglist();
	input->hashmap = load_environment(envp);
	input->special_sym = create_special_table();
	input->root = NULL;
	input->lexer.ch = '\0';
	input->lexer.input = NULL;
	input->lexer.input_len = 0;
	input->lexer.read_position = -1;
	input->lexer.tok_position = -1;
	input->unexpected = 0;
	return ;
}
