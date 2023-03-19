/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/19 12:17:36 by arobu             #+#    #+#             */
/*   Updated: 2023/03/19 16:10:12 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	accept(t_token *token, t_token_type type)
{
	if (token->type == type)
		return (1);
	return (0);
}

int	expect(t_token *token, t_token_type type)
{
	if (accept(token, type))
		return (1);
	return (0);
}

t_ast_node	*parse_command(t_token_list *tokens)
{
	t_token		*current_token;
	t_data		data;
	int			i;

	i = 0;
	current_token = tokens->first;
	data.command.name = NULL;
	data.command.arglist = NULL;
	if (accept(current_token, TOKEN_WORD))
	{
		data.command.name = ft_strdup(current_token->value.word.value);
		current_token = get_next_token(current_token);
		while (accept(current_token, TOKEN_WORD))
		{
			if (!data.command.arglist)
				data.command.arglist = new_arglist();
			new_argument(data.command.arglist, \
				create_arg(current_token->value.word.value));
			current_token = get_next_token(current_token);
		}
		return (new_node(data));
	}
	return (NULL);
}

int	parse_simple_cmd(t_token_list *tokens)
{
	return (0);
}

int	parse_prefix(t_token_list *tokens)
{
	return (0);
}

int	parse_io_file(t_token_list *tokens)
{
	t_token	*curr_token;

	return (0);
}
