/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fsm_tokenizer_heredoc.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 14:43:03 by arobu             #+#    #+#             */
/*   Updated: 2023/04/24 20:07:38 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"
#include <sys/wait.h>
#include "signals.h"

int	do_heredoc(t_token *token, t_fsm *fsm, t_input *input)
{
	char	*line;
	char	*filename;
	int		fd;

	line = NULL;
	filename = NULL;
	if (check_correct_token(token, fsm, input))
		return (1);
	get_file_name(token, input, &filename);
	fd = open(filename, O_CREAT | O_WRONLY | O_APPEND, 0644);
	if (fd > 0)
	{
		new_argument(input->heredoc_files, create_heredoc_file(filename));
		free(filename);
		while (1)
		{
			line = readline("heredoc> ");
			if (!line)
				return (1);
			if (run_heredoc(token, input, &fd, line))
				return (1);
		}
	}
	close(fd);
	return (0);
}

int	check_correct_token(t_token *token, t_fsm *fsm, t_input *input)
{
	if (!is_token_word_literal(token))
	{
		fsm->state = ERROR;
		input->unexpected = token->type;
		return (1);
	}
	return (0);
}

void	get_file_name(t_token *token, t_input *input, char **filename)
{
	if (token->type == TOKEN_WORD)
		*filename = get_env_vars(expand_vars(token->value.word.value), input);
	else if (token->type == TOKEN_QUOTE)
		*filename = get_env_vars(expand_vars(token->value.quote.value), input);
}

int	heredoc_token_word(char *line, t_token *token, t_input *input, int fd)
{
	char	*expanded;

	if (!ft_strncmp(line, token->value.word.value, \
		ft_strlen(token->value.word.value)))
	{
		free(line);
		return (1);
	}
	expanded = get_env_vars(expand_vars(line), input);
	ft_putstr_fd(expanded, fd);
	ft_putstr_fd("\n", fd);
	free(line);
	free(expanded);
	return (0);
}
