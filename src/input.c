/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/03 00:42:57 by arobu             #+#    #+#             */
/*   Updated: 2023/04/18 18:38:33 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "environment_handler.h"
#include "quote_list.h"

static int	is_valid_string(t_input *input);

int		parse_all_input(t_input *input)
{
	if (input->tokens->first->type == TOKEN_QUOTE && \
	input->tokens->num_tokens == 2 && \
		!ft_strncmp(input->tokens->first->value.quote.value, "\"\"", 3))
	{
		ft_putstr_fd("minishell: : command not found\n", 2);
		free(input->lexer.input);
		free_token_list(input->tokens);
		free(hashmap_put(input->special_sym, "EXITSTATUS", ft_itoa(127)));
		return (1);
	}
	while (input->tokens->first->type != TOKEN_EOF)
		parse_input(&input->root, input->tokens, input);
	return (0);
}

t_hashmap	*create_special_table(void)
{
	int			i;
	int			count;
	char		*home;
	char		*path;
	t_hashmap	*hashmap;

	hashmap = hashmap_new(100);
	path = getcwd(NULL, 0);
	i = 0;
	count = 0;
	while (path && path[i])
	{
		if (path[i] == '/')
			count++;
		if (count == 3)
			break ;
		i++;
	}
	home = ft_substr(path, 0, &path[i] - path);
	hashmap_put(hashmap, "TILDE", home);
	hashmap_put(hashmap, "EXITSTATUS", ft_strdup("0"));
	free(path);
	return (hashmap);
	// path = NULL;
}

void	init_input(t_input	*input, char **envp)
{
	input->tokens = new_token_list();
	input->hashmap = load_environment(envp);
	input->special_sym = create_special_table();
	input->root = NULL;
	input->lexer.ch = '\0';
	input->lexer.input = NULL;
	input->lexer.input_len = 0;
	input->lexer.read_position = -1;
	input->lexer.tok_position = -1;
	input->unexpected = 0;
	return ;
}

// char	*expand_env_var(char *value, t_input *input)
// {
// 	t_arglist	*list;
// 	t_arg		*arg;
// 	size_t		new_len;
// 	char		*new_value;
// 	char		*entry;
// 	int			prev_index;

// 	if (value)
// 	{
// 		list = expand_vars(value);
// 		new_len = ft_strlen(value);

// 	}
// 	else
// 		return (ft_strdup(""));
// 	if (list->arg_count == 0)
// 	{
// 		printf("EMPTY\n");
// 		// free_args(list);
// 		return (value);
// 	}
// 	// print_args(list);
// 	// printf("%s\n", value);
// 	arg = list->first;
// 	while (arg)
// 	{
// 		new_len -= arg->len;
// 		entry = (char *)hashmap_get(input->hashmap, arg->value);
// 		if (entry)
// 			new_len += ft_strlen(entry);
// 		else
// 			new_len += 0;
// 		arg = arg->next;
// 	}
// 	new_value = (char *)ft_calloc(new_len + 1, sizeof(char));
// 	if (!new_value)
// 		return (NULL);
// 	arg = list->first;
// 	// printf("WTF: %s\n", value);
// 	if (arg && arg->start_pos != 0)
// 		ft_strncat(new_value, &value[0], arg->start_pos + 1);
// 	// print_args(list);
// 	while (arg)
// 	{
// 		prev_index = arg->start_pos + arg->len;
// 		entry = (char *)hashmap_get(input->hashmap, arg->value);
// 		if (entry)
// 			ft_strcat(new_value, entry);
// 		else
// 			ft_strcat(new_value, "");
// 		arg = arg->next;
// 		if (arg)
// 			ft_strncat(new_value, &value[prev_index], arg->start_pos - prev_index);
// 	}
// 	if (ft_strlen(&value[prev_index]) != 0)
// 		ft_strncat(new_value, &value[prev_index], ft_strlen(&value[prev_index]));
// 	free_args(list);
// 	return (new_value);
// }

int	generate_input(t_input *input)
{
	t_token		*token;

	// if (init_lexer(input))
	// 	return (1);
	// while (is_valid_string(input))
	// {
	// 	free_token_list(input->tokens);
	// 	analyze_input(&input->lexer);
	// 	get_tokens(input->tokens, input->lexer);
	// 	token = get_last_token(input->tokens);
	// 	if (token && append_input_pipe(&input->lexer, token->type))
	// 		break ;
	// 	input->lexer.read_position = -1;
	// }
	// if (analyze_syntax(input->tokens, &input->unexpected) != 0)
	// {
	// 	ft_putstr_fd("incorrect syntax near", 2);
	// 	printf(" %d\n", input->unexpected);
	// 	free(input->lexer.input);
	// 	free_token_list(input->tokens);
	// 	return (2);
	// }
	print_tokens(input->tokens);
	return (0);
}

static int	is_valid_string(t_input *input)
{
	return (input->tokens->num_tokens == 0 || \
		(get_last_token(input->tokens)->type != TOKEN_WORD) && \
		get_last_token(input->tokens)->type != TOKEN_SEMICOLON);
}
