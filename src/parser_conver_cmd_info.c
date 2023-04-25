/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_conver_cmd_info.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 01:59:09 by arobu             #+#    #+#             */
/*   Updated: 2023/04/25 01:59:24 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"
#include "parser.h"
#include "x_execution.h"

void	parse_assignment(t_token_list *tokens, t_cmd_prefix *prefix)
{
	if (is_assign_word(tokens->first))
	{
		if (!prefix->assignments)
			prefix->assignments = new_arglist();
		new_argument(prefix->assignments, \
			create_arg((tokens)->first, NORMAL));
		consume_token(tokens);
	}
}

void	convert_info_to_cmd(t_command_info info, t_data *data)
{
	if (info.name)
		data->command.cmd.name_path = ft_strdup(info.name);
	data->command.input = get_input_file(info);
	data->command.output = get_output_file(info);
	get_cmd_args(info, data);
	get_assignments(info, data);
}

t_io_redirect	get_input_file(t_command_info info)
{
	t_io_redirect	input;

	input.fd_redir_out = 0;
	if (info.suffix.input.filename)
		input.filename = ft_strdup(info.suffix.input.filename);
	else if (info.prefix.input.filename)
		input.filename = ft_strdup(info.prefix.input.filename);
	else
		input.filename = NULL;
	return (input);
}

t_io_redirect	get_output_file(t_command_info info)
{
	t_io_redirect	output;

	if (info.suffix.output.filename)
	{
		output.filename = ft_strdup(info.suffix.output.filename);
		output.fd_redir_out = info.suffix.fd_redir_out;
		output.is_appended = info.suffix.output.is_appended;
	}
	else if (info.prefix.output.filename)
	{
		output.filename = ft_strdup(info.prefix.output.filename);
		output.is_appended = info.prefix.output.is_appended;
		output.fd_redir_out = info.prefix.fd_redir_out;
	}
	else
	{
		output.filename = NULL;
	}
	return (output);
}

void	get_assignments(t_command_info info, t_data *data)
{
	t_arg	*arg;

	arg = NULL;
	if (info.prefix.assignments)
	{
		arg = info.prefix.assignments->first;
		data->command.cmd.assignments = new_arglist();
	}
	while (arg)
	{
		new_argument(data->command.cmd.assignments, copy_arg(arg));
		arg = arg->next;
	}
}
