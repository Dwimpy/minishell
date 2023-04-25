/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_ast_nodes.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 02:53:10 by arobu             #+#    #+#             */
/*   Updated: 2023/04/25 02:54:13 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"
#include "parser.h"
#include "x_execution.h"

t_ast_node	*parse_command(t_token_list *tokens, \
			t_input *input, size_t sub_count)
{
	t_command_info	cmd;
	t_data			data;

	data.command.cmd.args = NULL;
	data.command.cmd.assignments = NULL;
	data.command.cmd.name_path = NULL;
	data.command.input.filename = NULL;
	data.command.output.filename = NULL;
	cmd.name = NULL;
	cmd.arglist = NULL;
	cmd.prefix = parse_prefix(&tokens, input);
	cmd.type = COMMAND;
	parse_cmd_word(&tokens, &cmd);
	cmd.suffix = parse_suffix(&tokens, input);
	convert_info_to_cmd(cmd, &data);
	free_cmd_info(cmd);
	return (new_node(data, COMMAND, sub_count));
}

t_ast_node	*parse_pipeline(t_token_list *tokens, size_t sub_count)
{
	t_data		data;

	data.pipeline.symbol = '|';
	if (tokens->first->type == TOKEN_PIPE)
	{
		consume_token(tokens);
		return (new_node(data, PIPELINE, sub_count));
	}
	return (NULL);
}

t_ast_node	*parse_and_if(t_token_list *tokens, size_t sub_count)
{
	t_data		data;

	data.and_if.symbol = NULL;
	if (tokens->first->type == TOKEN_AND_IF)
	{
		data.and_if.symbol = ft_strdup("&&");
		consume_token(tokens);
		return (new_node(data, AND_IF, sub_count));
	}
	return (NULL);
}

t_ast_node	*parse_or_if(t_token_list *tokens, size_t sub_count)
{
	t_data		data;

	data.or_if.symbol = NULL;
	if (tokens->first->type == TOKEN_OR_IF)
	{
		data.and_if.symbol = ft_strdup("||");
		consume_token(tokens);
		return (new_node(data, OR_IF, sub_count));
	}
	return (NULL);
}

t_ast_node	*parse_subshell(t_token_list *tokens, size_t *sub_count)
{
	while (accept(tokens->first, TOKEN_LPARENTHESIS) || \
		accept(tokens->first, TOKEN_RPARENTHESIS))
	{
		if (accept(tokens->first, TOKEN_LPARENTHESIS))
			(*sub_count)++;
		else if (accept(tokens->first, TOKEN_RPARENTHESIS))
			(*sub_count)--;
		consume_token(tokens);
	}
	return (NULL);
}
