/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_prefix_redir.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 01:43:00 by arobu             #+#    #+#             */
/*   Updated: 2023/04/25 01:49:08 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"
#include "parser.h"
#include "x_execution.h"

static void	get_prefix_values(t_cmd_prefix *prefix, t_token_list *tokens);
static void	set_prefix_type(t_token_list *tokens, \
		t_cmd_prefix *prefix, t_input *input);
static void	parse_prefix_redirection(t_token_list *tokens, \
		t_cmd_prefix *prefix, t_input *input);

void	parse_redirection_prefix(t_token_list *tokens, \
			t_cmd_prefix *prefix, t_input *input)
{
	if (accept_redirection((tokens)->first))
		parse_prefix_redirection(tokens, prefix, input);
}

static void	parse_prefix_redirection(t_token_list *tokens, \
		t_cmd_prefix *prefix, t_input *input)
{
	if (is_input_redir((tokens)->first))
	{
		set_prefix_type(tokens, prefix, input);
		consume_token(tokens);
	}
	else if (is_output_redir((tokens)->first))
	{
		get_prefix_values(prefix, tokens);
		consume_token(tokens);
		if (prefix->output.is_appended)
			create_and_free((tokens)->first, \
				&prefix->output.filename, OUTPUT_APPEND, input);
		else
			create_and_free((tokens)->first, \
				&prefix->output.filename, OUTPUT, input);
		consume_token(tokens);
	}
}

static void	set_prefix_type(t_token_list *tokens, \
		t_cmd_prefix *prefix, t_input *input)
{
	if ((tokens)->first->type == TOKEN_DLESS)
	{
		consume_token(tokens);
		create_and_free((tokens)->first, \
			&prefix->input.filename, HERE_DOC, input);
	}
	else
	{
		consume_token(tokens);
		create_and_free((tokens)->first, \
			&prefix->input.filename, INPUT, input);
	}
}

static void	get_prefix_values(t_cmd_prefix *prefix, t_token_list *tokens)
{
	if (tokens->first->type == TOKEN_DGREAT)
		prefix->output.is_appended = 1;
	else
		prefix->output.is_appended = 0;
	if (tokens->first->type == TOKEN_GREAT && \
			tokens->first->value.great.from == STD_IN)
		prefix->fd_redir_out = 0;
	else if (tokens->first->type == TOKEN_DGREAT && \
			tokens->first->value.dgreat.from == STD_IN)
		prefix->fd_redir_out = 0;
	else if (tokens->first->type == TOKEN_GREAT && \
			tokens->first->value.great.from == STD_OUT)
		prefix->fd_redir_out = 1;
	else if (tokens->first->type == TOKEN_DGREAT && \
			tokens->first->value.dgreat.from == STD_OUT)
		prefix->fd_redir_out = 1;
	else if (tokens->first->type == TOKEN_GREAT && \
			tokens->first->value.great.from == STD_ERR)
		prefix->fd_redir_out = 2;
	else if (tokens->first->type == TOKEN_DGREAT && \
			tokens->first->value.dgreat.from == STD_ERR)
		prefix->fd_redir_out = 2;
}
