/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_helper_funcs.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 01:58:23 by arobu             #+#    #+#             */
/*   Updated: 2023/04/25 02:50:53 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"
#include "parser.h"
#include "x_execution.h"

void	parse_cmd_word(t_token_list	**tokens, t_command_info *data)
{
	if (is_cmd_word((*tokens)->first) || is_assign_word((*tokens)->first))
	{
		data->name = ft_strdup(get_token_value((*tokens)->first));
		consume_token((*tokens));
		while (is_cmd_word((*tokens)->first) || \
				is_assign_word((*tokens)->first))
		{
			if (!data->arglist)
				data->arglist = new_arglist();
			if ((*tokens)->first->type == TOKEN_WORD || \
				(*tokens)->first->type == TOKEN_ASSIGN_WORD)
			{
				new_argument(data->arglist, \
					create_arg((*tokens)->first, NORMAL));
				consume_token(*tokens);
			}
			else if ((*tokens)->first->type == TOKEN_QUOTE)
			{
				new_argument(data->arglist, \
					create_arg((*tokens)->first, QUOTED_ARG));
				consume_token(*tokens);
			}
		}
	}
}

t_cmd_prefix	parse_prefix(t_token_list **tokens, t_input *input)
{
	t_cmd_prefix	prefix;

	prefix.input.filename = NULL;
	prefix.output.filename = NULL;
	prefix.assignments = NULL;
	prefix.fd_redir_out = 3;
	while (is_prefix((*tokens)->first))
	{
		parse_redirection_prefix(*tokens, &prefix, input);
		parse_assignment(*tokens, &prefix);
	}
	return (prefix);
}

t_cmd_suffix	parse_suffix(t_token_list **tokens, t_input *input)
{
	t_cmd_suffix	suffix;

	suffix.input.filename = NULL;
	suffix.output.filename = NULL;
	suffix.arglist = NULL;
	suffix.fd_redir_out = 3;
	while (is_cmd_suffix((*tokens)->first))
	{
		parse_redirection_suffix(*tokens, &suffix, input);
		parse_suffix_words(*tokens, &suffix);
	}
	return (suffix);
}

void	parse_suffix_words(t_token_list *tokens, t_cmd_suffix *suffix)
{
	while (is_cmd_word((tokens)->first) || is_assign_word((tokens)->first))
	{
		if (!suffix->arglist)
			suffix->arglist = new_arglist();
		if ((tokens)->first->type == TOKEN_WORD || \
			(tokens)->first->type == TOKEN_ASSIGN_WORD)
		{
			new_argument(suffix->arglist, \
				create_arg((tokens)->first, NORMAL));
			consume_token(tokens);
		}
		else if ((tokens)->first->type == TOKEN_QUOTE)
		{
			new_argument(suffix->arglist, \
				create_arg((tokens)->first, QUOTED_ARG));
			consume_token(tokens);
		}
	}
}
