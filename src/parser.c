/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/19 12:17:36 by arobu             #+#    #+#             */
/*   Updated: 2023/03/26 19:23:17 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_ast_node	*parse_command(t_token_list *tokens)
{
	t_data		data;
	int			i;

	i = 0;
	if (tokens->first->type == TOKEN_EOF)
		return (NULL);
	data.command.name = NULL;
	data.command.arglist = NULL;
	data.command.prefix = parse_prefix(&tokens);
	data.command.type = COMMAND;
	parse_cmd_word(&tokens, &data);
	data.command.suffix = parse_suffix(&tokens);
	return (new_node(data, COMMAND));
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

	data.and_if.symbol = ft_strdup("&&");
	if (tokens->first->type == TOKEN_AND_IF)
	{
		consume_token(tokens);
		return (new_node(data, AND_IF));
	}
	return (NULL);
}

t_ast_node	*parse_or_if(t_token_list *tokens)
{
	t_data		data;

	data.and_if.symbol = ft_strdup("||");
	if (tokens->first->type == TOKEN_OR_IF)
	{
		consume_token(tokens);
		return (new_node(data, OR_IF));
	}
	return (NULL);
}

t_ast_node	*parse_subshell(t_token_list *tokens)
{
	t_ast_node	*root;
	t_ast_node	*subshell;
	t_data		data;

	subshell = NULL;
	root = NULL;
	data.subshell.input = NULL;
	if (accept(tokens->first, TOKEN_LPARENTHESIS))
	{
		consume_token(tokens);
		while (!accept(tokens->first, TOKEN_RPARENTHESIS))
		{
			ast_add(&root, parse_command(tokens));
			ast_add(&root, parse_pipeline(tokens));
			ast_add(&root, parse_and_if(tokens));
			ast_add(&root, parse_or_if(tokens));
		}
		consume_token(tokens);
		subshell = new_node(data, SUBSHELL);
		subshell->left = root;
		return (subshell);
	}
	return (root);
}

void	parse_cmd_word(t_token_list	**tokens, t_data *data)
{
	if (is_cmd_word((*tokens)->first))
	{
		data->command.name = ft_strdup(get_token_value((*tokens)->first));
		consume_token((*tokens));
		while (is_cmd_word((*tokens)->first))
		{
			if (!data->command.arglist)
				data->command.arglist = new_arglist();
			new_argument(data->command.arglist, \
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
	while (is_cmd_word((tokens)->first))
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
		prefix->assignments = new_arglist();
		new_argument(prefix->assignments, \
			create_arg((tokens)->first));
		consume_token(tokens);
	}
}
