/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_helper_funcs_2.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 02:00:07 by arobu             #+#    #+#             */
/*   Updated: 2023/04/25 02:02:00 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"
#include "parser.h"
#include "x_execution.h"

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
