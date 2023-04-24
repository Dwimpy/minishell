/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/19 12:17:36 by arobu             #+#    #+#             */
/*   Updated: 2023/04/25 00:34:09 by arobu            ###   ########.fr       */
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

void	parse_input(t_ast_node **root, t_token_list *tokens, \
		t_input *input, size_t	*sub_count)
{
	parse_subshell(tokens, sub_count);
	ast_add(root, parse_command(tokens, input, *sub_count));
	parse_subshell(tokens, sub_count);
	ast_add(root, parse_pipeline(tokens, *sub_count));
	parse_subshell(tokens, sub_count);
	ast_add(root, parse_and_if(tokens, *sub_count));
	parse_subshell(tokens, sub_count);
	ast_add(root, parse_or_if(tokens, *sub_count));
	parse_subshell(tokens, sub_count);
	ast_add(root, parse_subshell(tokens, sub_count));
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

void	parse_redirection_prefix(t_token_list *tokens, \
			t_cmd_prefix *prefix, t_input *input)
{
	if (accept_redirection((tokens)->first))
	{
		if (is_input_redir((tokens)->first))
		{
			if ((tokens)->first->type == TOKEN_DLESS)
			{
				consume_token(tokens);
				create_and_free((tokens)->first, &prefix->input.filename, HERE_DOC, input);
			}
			else
			{
				consume_token(tokens);
				create_and_free((tokens)->first, &prefix->input.filename, INPUT, input);
				
			}
			consume_token(tokens);
		}
		else if (is_output_redir((tokens)->first))
		{
			if (tokens->first->type == TOKEN_DGREAT)
				prefix->output.is_appended = 1;
			else
				prefix->output.is_appended = 0;
			if (tokens->first->type == TOKEN_GREAT && tokens->first->value.great.from == STD_IN)
				prefix->fd_redir_out = 0;
			else if (tokens->first->type == TOKEN_DGREAT && tokens->first->value.dgreat.from == STD_IN)
				prefix->fd_redir_out = 0;
			else if (tokens->first->type == TOKEN_GREAT && tokens->first->value.great.from == STD_OUT)
				prefix->fd_redir_out = 1;
			else if (tokens->first->type == TOKEN_DGREAT && tokens->first->value.dgreat.from == STD_OUT)
				prefix->fd_redir_out = 1;
			else if (tokens->first->type == TOKEN_GREAT && tokens->first->value.great.from == STD_ERR)
				prefix->fd_redir_out = 2;
			else if (tokens->first->type == TOKEN_DGREAT && tokens->first->value.dgreat.from == STD_ERR)
				prefix->fd_redir_out = 2;
			consume_token(tokens);
			if (prefix->output.is_appended)
				create_and_free((tokens)->first, &prefix->output.filename, OUTPUT_APPEND, input);
			else
				create_and_free((tokens)->first, &prefix->output.filename, OUTPUT, input);
			consume_token(tokens);
		}
	}
}

void	parse_redirection_suffix(t_token_list *tokens, t_cmd_suffix *suffix, t_input *input)
{
	if (accept_redirection((tokens)->first))
	{
		if (is_input_redir((tokens)->first))
		{
			if ((tokens)->first->type == TOKEN_DLESS)
			{
				consume_token(tokens);
				create_and_free((tokens)->first, &suffix->input.filename, HERE_DOC, input);
			}
			else
			{
				consume_token(tokens);
				create_and_free((tokens)->first, &suffix->input.filename, INPUT, input);
			}
			consume_token(tokens);
		}
		else if (is_output_redir((tokens)->first))
		{
			if (tokens->first->type == TOKEN_DGREAT)
				suffix->output.is_appended = 1;
			else
				suffix->output.is_appended = 0;
			if (tokens->first->type == TOKEN_GREAT && tokens->first->value.great.from == STD_IN)
				suffix->fd_redir_out = 0;
			else if (tokens->first->type == TOKEN_DGREAT && tokens->first->value.dgreat.from == STD_IN)
				suffix->fd_redir_out = 0;
			else if (tokens->first->type == TOKEN_GREAT && tokens->first->value.great.from == STD_OUT)
				suffix->fd_redir_out = 1;
			else if (tokens->first->type == TOKEN_DGREAT && tokens->first->value.dgreat.from == STD_OUT)
				suffix->fd_redir_out = 1;
			else if (tokens->first->type == TOKEN_GREAT && tokens->first->value.great.from == STD_ERR)
				suffix->fd_redir_out = 2;
			else if (tokens->first->type == TOKEN_DGREAT && tokens->first->value.dgreat.from == STD_ERR)
				suffix->fd_redir_out = 2;
			consume_token(tokens);
			if (suffix->output.is_appended)
				create_and_free((tokens)->first, &suffix->output.filename, OUTPUT_APPEND, input);
			else
				create_and_free((tokens)->first, &suffix->output.filename, OUTPUT, input);
			consume_token(tokens);
		}
	}
}

void	create_and_free(t_token *token, char **filename, int io, t_input *input)
{
	int		fd;

	fd = 0;
	if (!*filename)
	{
		if (token->type == TOKEN_QUOTE)
			*filename = get_env_vars(expand_vars(token->value.quote.value), input);
		else if (token->type == TOKEN_WORD)
			*filename = get_env_vars(expand_vars(token->value.word.value), input);
	}
	else
	{
		free(*filename);
		if (token->type == TOKEN_QUOTE)
			*filename = get_env_vars(expand_vars(token->value.quote.value), input);
		else if (token->type == TOKEN_WORD)
			*filename = get_env_vars(expand_vars(token->value.word.value), input);
	}
	if (io == INPUT)
	{
		if (!ft_strncmp(*filename, "./", 2))
			fd = open(&(*filename)[2], O_RDONLY);
		else
			fd = open((*filename), O_RDONLY);
	}
	else if (io == HERE_DOC)
	{
		fd = open(*filename, O_CREAT | O_WRONLY | O_APPEND, 0644);
		if (fd > 0)
			new_argument(input->heredoc_files, create_heredoc_file(*filename));
	}
	else if (io == OUTPUT)
	{
		if (!ft_strncmp(*filename, "./", 2))
			fd = open(*filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		else
			fd = open(*filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	}
	else if (io == OUTPUT_APPEND)
	{
		if (!ft_strncmp(*filename, "./", 2))
			fd = open(*filename, O_CREAT | O_WRONLY | O_APPEND, 0644);
		else
			fd = open(*filename, O_CREAT | O_WRONLY | O_APPEND, 0644);
	}
	if (fd < 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(*filename, 2);
		ft_putendl_fd(": No such file or directory", 2);
	}
	close(fd);
}

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
		}
		if (info.suffix.arglist)
			size += info.suffix.arglist->arg_count;
		data->command.cmd.args = (char **)ft_calloc(size + 1, sizeof(char *));
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
	if (info.suffix.input.filename)
		free(info.suffix.input.filename);
	if (info.suffix.output.filename)
		free(info.suffix.output.filename);
	if (info.prefix.assignments)
		free_args(info.prefix.assignments);
	if (info.suffix.arglist)
		free_args(info.suffix.arglist);
}

int	is_valid_beginning(t_token *token)
{
	return (is_cmd_word(token) || \
			is_input_redir(token) || \
				is_output_redir(token) || \
					token->type == TOKEN_LPARENTHESIS || \
						token->type == TOKEN_ASSIGN_WORD);
}
