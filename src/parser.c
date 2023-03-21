/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/19 12:17:36 by arobu             #+#    #+#             */
/*   Updated: 2023/03/21 13:48:12 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

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

	prefix.input.filename = NULL;
	prefix.output.filename = NULL;
	prefix.assignments.arglist = NULL;
	while (is_prefix((*tokens)->first))
	{
		parse_redirection((*tokens), &prefix);
		parse_assignment((*tokens), &prefix);
	}
	return (prefix);
}

void	parse_redirection(t_token_list *tokens, t_cmd_prefix *prefix)
{
	if (accept_redirection((tokens)->first))
	{
		if (is_input_redir((tokens)->first))
		{
			consume_token(tokens);
			prefix->input.filename = \
				ft_strdup((tokens)->first->value.word.value);
			consume_token(tokens);
		}
		else if (is_output_redir((tokens)->first))
		{
			consume_token(tokens);
			prefix->output.filename = \
				ft_strdup((tokens)->first->value.word.value);
			consume_token(tokens);
		}
	}
}

void	parse_assignment(t_token_list *tokens, t_cmd_prefix *prefix)
{
	if (is_assign_word(tokens->first))
	{
		if (!prefix->assignments.arglist)
			prefix->assignments.arglist = new_arglist();
		new_argument(prefix->assignments.arglist, \
			create_arg(tokens->first->value.word.value));
		consume_token(tokens);
	}
}

t_cmd_suffix	parse_suffix(t_token **token)
{
	t_cmd_suffix	suffix;

	return (suffix);
}

int	parse_io_file(t_token *token)
{
	t_token	*curr_token;

	return (0);
}
