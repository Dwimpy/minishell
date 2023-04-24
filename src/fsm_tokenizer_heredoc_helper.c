/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fsm_tokenizer_heredoc_helper.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 20:07:30 by arobu             #+#    #+#             */
/*   Updated: 2023/04/24 22:10:12 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"
#include <sys/wait.h>
#include "signals.h"

int	run_heredoc(t_token *token, t_input *input, int *fd, char *line)
{
	if (token->type == TOKEN_WORD)
	{
		if (heredoc_token_word(line, token, input, *fd))
			return (1);
	}
	else if (token->type == TOKEN_QUOTE)
	{
		if (heredoc_token_quote(line, token, input, *fd))
			return (1);
	}
	return (0);
}

int	heredoc_token_quote(char *line, t_token *token, t_input *input, int fd)
{
	char	*str;
	char	*expanded;

	str = get_env_vars(expand_vars(token->value.quote.value), input);
	if (!ft_strncmp(line, str, ft_strlen(str) + 1))
	{
		free(str);
		free(line);
		return (1);
	}
	expanded = get_env_vars(expand_vars(line), input);
	ft_putstr_fd(expanded, fd);
	ft_putstr_fd("\n", fd);
	free(line);
	free(str);
	free(expanded);
	return (0);
}
