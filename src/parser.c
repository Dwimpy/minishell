/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/19 12:17:36 by arobu             #+#    #+#             */
/*   Updated: 2023/03/20 18:06:39 by arobu            ###   ########.fr       */
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

int	accept_redirection(t_token *token)
{
	if (accept(token, TOKEN_LESS) || accept(token, TOKEN_GREAT) || \
	accept(token, TOKEN_DGREAT) || accept(token, TOKEN_DLESS))
	{
		if (accept(token->next, TOKEN_WORD))
			return (1);
	}
	return (0);
}

t_ast_node	*parse_command(t_token_list *tokens)
{
	t_token		*current_token;
	t_data		data;
	int			i;

	i = 0;
	data.command.name = NULL;
	data.command.arglist = NULL;
	data.command.prefix = parse_prefix(&tokens);
	current_token = tokens->first;
	if (accept(current_token, TOKEN_WORD))
	{
		if (!data.command.name)
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
	}
	return (new_node(data));
}

t_cmd_prefix	parse_prefix(t_token_list **tokens)
{
	t_cmd_prefix	prefix;
	t_token			*curr_token;

	prefix.input.filename = NULL;
	prefix.output.filename = NULL;
	prefix.assignments.arglist = NULL;
	while ((*tokens)->first->type == TOKEN_LESS ||	(*tokens)->first->type == TOKEN_DLESS || \
			(*tokens)->first->type == TOKEN_GREAT || (*tokens)->first->type == TOKEN_DGREAT || \
			(*tokens)->first->type == TOKEN_ASSIGN_WORD)
	{
	if (accept_redirection((*tokens)->first))
	{
		if ((*tokens)->first->type == TOKEN_LESS || (*tokens)->first->type == TOKEN_DLESS)
		{
			consume_token(*tokens);
			prefix.input.filename = ft_strdup((*tokens)->first->value.word.value);
			consume_token(*tokens);
		}
		else
		{
			consume_token(*tokens);
			prefix.output.filename = ft_strdup((*tokens)->first->value.word.value);
			consume_token(*tokens);
		}
	}
	}
	return (prefix);
}

t_cmd_suffix		parse_suffix(t_token **token)
{
	t_cmd_suffix	suffix;

	return (suffix);
}

int	parse_io_file(t_token *token)
{
	t_token	*curr_token;

	return (0);
}
