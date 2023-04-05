/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/19 12:17:36 by arobu             #+#    #+#             */
/*   Updated: 2023/04/05 23:45:59 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_ast_node	*parse_command(t_token_list *tokens, t_input *input)
{
	t_command_info	cmd;
	t_data			data;
	int				i;

	i = 0;
	if (is_prev_subshell(tokens->first))
		return (NULL);
	data.command.cmd.args = NULL;
	data.command.cmd.assignments = NULL;
	data.command.cmd.name_path = NULL;
	data.command.input.filename = NULL;
	data.command.output.filename = NULL;
	cmd.name = NULL;
	cmd.arglist = NULL;
	cmd.prefix = parse_prefix(&tokens);
	cmd.type = COMMAND;
	parse_cmd_word(&tokens, &cmd);
	cmd.suffix = parse_suffix(&tokens);
	print_args(cmd.arglist);
	print_args(cmd.suffix.arglist);
	convert_info_to_cmd(cmd, &data, input);
	free_cmd_info(cmd);
	return (new_node(data, COMMAND));
}

int	is_prev_subshell(t_token *token)
{
	return (token->type == TOKEN_PIPE || token->type == TOKEN_AND_IF || \
		token->type == TOKEN_OR_IF || token->type == TOKEN_LPARENTHESIS);
}

t_ast_node	*parse_pipeline(t_token_list *tokens)
{
	t_data		data;

	data.pipeline.symbol = '|';
	if (tokens->first->type == TOKEN_PIPE)
	{
		consume_token(tokens);
		return (new_node(data, PIPELINE));
	}
	return (NULL);
}

t_ast_node	*parse_and_if(t_token_list *tokens)
{
	t_data		data;

	data.and_if.symbol = NULL;
	if (tokens->first->type == TOKEN_AND_IF)
	{
		data.and_if.symbol = ft_strdup("&&");
		consume_token(tokens);
		return (new_node(data, AND_IF));
	}
	return (NULL);
}

t_ast_node	*parse_or_if(t_token_list *tokens)
{
	t_data		data;

	data.or_if.symbol = NULL;
	if (tokens->first->type == TOKEN_OR_IF)
	{
		data.and_if.symbol = ft_strdup("||");
		consume_token(tokens);
		return (new_node(data, OR_IF));
	}
	return (NULL);
}

t_ast_node	*parse_subshell(t_token_list *tokens, t_input *input)
{
	t_ast_node	*root;
	t_ast_node	*subshell;
	t_data		data;

	subshell = NULL;
	root = NULL;
	if (accept(tokens->first, TOKEN_LPARENTHESIS))
	{
		consume_token(tokens);
		while (!accept(tokens->first, TOKEN_RPARENTHESIS))
		{
			ast_add(&root, parse_command(tokens, input));
			ast_add(&root, parse_pipeline(tokens));
			ast_add(&root, parse_and_if(tokens));
			ast_add(&root, parse_or_if(tokens));
			ast_add(&root, parse_subshell(tokens, input));
		}
		root->is_subshell = 1;
		consume_token(tokens);
		return (root);
	}
	return (root);
}

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
			new_argument(data->arglist, \
				create_arg((*tokens)->first));
			consume_token(*tokens);
		}
	}
}

t_cmd_prefix	parse_prefix(t_token_list **tokens)
{
	t_cmd_prefix	prefix;

	prefix.input.filename = NULL;
	prefix.output.filename = NULL;
	prefix.assignments = NULL;
	while (is_prefix((*tokens)->first))
	{
		parse_redirection_prefix(*tokens, &prefix);
		parse_assignment(*tokens, &prefix);
	}
	return (prefix);
}

t_cmd_suffix	parse_suffix(t_token_list **tokens)
{
	t_cmd_suffix	suffix;

	suffix.input.filename = NULL;
	suffix.output.filename = NULL;
	suffix.arglist = NULL;
	while (is_cmd_suffix((*tokens)->first))
	{
		parse_redirection_suffix(*tokens, &suffix);
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
		new_argument(suffix->arglist, \
			create_arg((tokens)->first));
		consume_token(tokens);
	}
}

void	parse_redirection_prefix(t_token_list *tokens, \
			t_cmd_prefix *prefix)
{
	if (accept_redirection((tokens)->first))
	{
		if (is_input_redir((tokens)->first))
		{
			consume_token(tokens);
			create_and_free((tokens)->first, &prefix->input.filename, INPUT);
			consume_token(tokens);
		}
		else if (is_output_redir((tokens)->first))
		{
			consume_token(tokens);
			create_and_free((tokens)->first, &prefix->output.filename, OUTPUT);
			consume_token(tokens);
		}
	}
}

void	parse_redirection_suffix(t_token_list *tokens, t_cmd_suffix *suffix)
{
	if (accept_redirection((tokens)->first))
	{
		if (is_input_redir((tokens)->first))
		{
			consume_token(tokens);
			create_and_free((tokens)->first, &suffix->input.filename, INPUT);
			consume_token(tokens);
		}
		else if (is_output_redir((tokens)->first))
		{
			consume_token(tokens);
			create_and_free((tokens)->first, &suffix->output.filename, OUTPUT);
			consume_token(tokens);
		}
	}
}

void	create_and_free(t_token *token, char **filename, int io)
{
	int	fd;

	if (!*filename)
		*filename = ft_strdup(get_token_value(token));
	else
	{
		free(*filename);
		*filename = ft_strdup(get_token_value(token));
	}
	if (io == INPUT)
		fd = open(*filename, O_RDONLY);
	else if (io == OUTPUT)
		fd = open(*filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
		perror("error");
	close(fd);
}

void	parse_assignment(t_token_list *tokens, t_cmd_prefix *prefix)
{
	if (is_assign_word(tokens->first))
	{
		if (!prefix->assignments)
			prefix->assignments = new_arglist();
		new_argument(prefix->assignments, \
			create_arg((tokens)->first));
		consume_token(tokens);
	}
}

void	parse_input(t_ast_node **root, t_token_list *tokens, t_input *input)
{
	ast_add(root, parse_command(tokens, input));
	ast_add(root, parse_pipeline(tokens));
	ast_add(root, parse_and_if(tokens));
	ast_add(root, parse_or_if(tokens));
	ast_add(root, parse_subshell(tokens, input));
}

void	convert_info_to_cmd(t_command_info info, t_data *data, t_input *input)
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

	if (info.suffix.input.filename)
		input.filename = ft_strdup(info.prefix.input.filename);
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
		output.filename = ft_strdup(info.prefix.output.filename);
	else if (info.prefix.output.filename)
		output.filename = ft_strdup(info.prefix.output.filename);
	else
		output.filename = NULL;
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

void	get_cmd_args(t_command_info info, t_data *data)
{
	t_arg	*arg;
	size_t	size;
	int		i;

	size = 0;
	i = 0;
	arg = NULL;
	if (info.name)
	{
		size += 1;
		if (info.arglist)
		{
			arg = info.arglist->first;
			size += info.arglist->arg_count;
			if (info.suffix.arglist)
				size += info.suffix.arglist->arg_count;
		}
		data->command.cmd.args = (char **)malloc(sizeof(char *) * (size + 1));
		data->command.cmd.args[i] = ft_strdup(data->command.cmd.name_path);
		i++;
	}
	while (arg)
	{
		data->command.cmd.args[i] = ft_strdup(arg->value);
		arg = arg->next;
		i++;
	}
	if (info.suffix.arglist)
		arg = info.suffix.arglist->first;
	while (arg)
	{
		data->command.cmd.args[i] = ft_strdup(arg->value);
		arg = arg->next;
		i++;
	}
	if (data->command.cmd.args)
		data->command.cmd.args[i] = NULL;
}

void	free_cmd_info(t_command_info info)
{
	if (info.name)
		free(info.name);
	if (info.arglist)
		free_args(info.arglist);
	if (info.prefix.input.filename)
		free(info.prefix.input.filename);
	if (info.prefix.output.filename)
		free(info.prefix.output.filename);
	if (info.suffix.output.filename)
		free(info.suffix.output.filename);
	if (info.prefix.assignments)
		free_args(info.prefix.assignments);
	if (info.suffix.arglist)
		free_args(info.suffix.arglist);
}
