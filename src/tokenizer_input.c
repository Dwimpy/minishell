/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_input.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/23 20:53:34 by arobu             #+#    #+#             */
/*   Updated: 2023/04/24 20:11:39 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"
#include <sys/wait.h>
#include "signals.h"

static int	check_lexer_input(t_input *input);

int	get_tokenizer_input(t_input *input)
{
	input->lexer.input = read_from_stdin(input);
	if (check_lexer_input(input))
		return (1);
	return (0);
}

static int	check_lexer_input(t_input *input)
{
	int		status;
	int		i;

	status = 0;
	i = 0;
	if (!input->lexer.input)
	{
		status = ft_atoi((char *)hashmap_get(input->special_sym, "EXITSTATUS"));
		free_input_at_exit(input);
		exit(status);
	}
	input->lexer.input_len = ft_strlen(input->lexer.input);
	while (input->lexer.input && (input->lexer.input[i] == ' ' || \
		input->lexer.input[i] == '\t'))
		i++;
	if (i == input->lexer.input_len)
	{
		free(input->lexer.input);
		status = 1;
	}
	input->lexer.read_position = -1;
	input->lexer.tok_position = -1;
	input->unexpected = 0;
	return (status);
}

int	handle_tokenizer_errors(t_input *input, t_fsm *fsm)
{
	if (fsm->state == ERROR)
	{
		free_input_on_error(input);
		return (1);
	}
	if (fsm->state == COMPLETE)
		add_token(input->tokens, new_token(TOKEN_EOF, NULL, EMPTY));
	return (0);
}

void	free_input_at_exit(t_input *input)
{
	free(input->tokens);
	free(input->heredoc_files);
	hashmap_free(&input->hashmap);
	hashmap_free(&input->special_sym);
}

void	free_input_on_error(t_input *input)
{
	print_syntax_error(input->unexpected);
	free(input->lexer.input);
	free_token_list(input->tokens);
	free(hashmap_put(input->special_sym, "EXITSTATUS", ft_strdup("2")));
	free_heredoc_list(input->heredoc_files);
}
