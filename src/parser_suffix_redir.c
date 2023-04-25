/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_suffix_redir.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 01:49:44 by arobu             #+#    #+#             */
/*   Updated: 2023/04/25 02:51:24 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"
#include "parser.h"
#include "x_execution.h"

static void	get_suffix_values(t_cmd_suffix *suffix, t_token_list *tokens);
static void	set_suffix_type(t_token_list *tokens, \
		t_cmd_suffix *suffix, t_input *input);
static void	parse_suffix_redirection(t_token_list *tokens, \
		t_cmd_suffix *prefix, t_input *input);

void	parse_redirection_suffix(t_token_list *tokens, \
					t_cmd_suffix *suffix, t_input *input)
{
	if (accept_redirection((tokens)->first))
		parse_suffix_redirection(tokens, suffix, input);
}

static void	parse_suffix_redirection(t_token_list *tokens, \
		t_cmd_suffix *suffix, t_input *input)
{
	if (is_input_redir((tokens)->first))
	{
		set_suffix_type(tokens, suffix, input);
		consume_token(tokens);
	}
	else if (is_output_redir((tokens)->first))
	{
		get_suffix_values(suffix, tokens);
		consume_token(tokens);
		if (suffix->output.is_appended)
			create_and_free((tokens)->first, \
				&suffix->output.filename, OUTPUT_APPEND, input);
		else
			create_and_free((tokens)->first, \
				&suffix->output.filename, OUTPUT, input);
		consume_token(tokens);
	}
}

static void	set_suffix_type(t_token_list *tokens, \
		t_cmd_suffix *suffix, t_input *input)
{
	if ((tokens)->first->type == TOKEN_DLESS)
	{
		consume_token(tokens);
		create_and_free((tokens)->first, \
			&suffix->input.filename, HERE_DOC, input);
	}
	else
	{
		consume_token(tokens);
		create_and_free((tokens)->first, \
			&suffix->input.filename, INPUT, input);
	}
}

static void	get_suffix_values(t_cmd_suffix *suffix, t_token_list *tokens)
{
	if (tokens->first->type == TOKEN_DGREAT)
		suffix->output.is_appended = 1;
	else
		suffix->output.is_appended = 0;
	if (tokens->first->type == TOKEN_GREAT && \
			tokens->first->value.great.from == STD_IN)
		suffix->fd_redir_out = 0;
	else if (tokens->first->type == TOKEN_DGREAT && \
			tokens->first->value.dgreat.from == STD_IN)
		suffix->fd_redir_out = 0;
	else if (tokens->first->type == TOKEN_GREAT && \
			tokens->first->value.great.from == STD_OUT)
		suffix->fd_redir_out = 1;
	else if (tokens->first->type == TOKEN_DGREAT && \
			tokens->first->value.dgreat.from == STD_OUT)
		suffix->fd_redir_out = 1;
	else if (tokens->first->type == TOKEN_GREAT && \
			tokens->first->value.great.from == STD_ERR)
		suffix->fd_redir_out = 2;
	else if (tokens->first->type == TOKEN_DGREAT && \
			tokens->first->value.dgreat.from == STD_ERR)
		suffix->fd_redir_out = 2;
}
