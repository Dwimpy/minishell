/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/23 18:59:19 by arobu             #+#    #+#             */
/*   Updated: 2023/04/25 02:51:48 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"
#include <sys/wait.h>
#include "signals.h"

void	readline_new_line(t_lexer *lexer, char *prompt)
{
	char	*append_line;
	char	*join_line;

	append_line = readline(prompt);
	if (!append_line)
		return ;
	join_line = (char *)malloc(sizeof(char) * (ft_strlen(lexer->input) + \
				ft_strlen(append_line) + 2));
	ft_strcpy(join_line, lexer->input);
	ft_strcat(join_line, "\n");
	ft_strcat(join_line, append_line);
	free(lexer->input);
	free(append_line);
	lexer->input = join_line;
	lexer->input_len = ft_strlen(lexer->input);
	lexer->read_position--;
}

int	readline_no_new_line(t_lexer *lexer, char *prompt)
{
	char	*append_line;
	char	*join_line;

	append_line = readline(prompt);
	if (!append_line)
		return (1);
	join_line = (char *)malloc(sizeof(char) * (ft_strlen(lexer->input) + \
				ft_strlen(append_line) + 2));
	ft_strcpy(join_line, lexer->input);
	ft_strcat(join_line, append_line);
	free(lexer->input);
	free(append_line);
	lexer->input = join_line;
	lexer->input_len = ft_strlen(lexer->input);
	lexer->read_position--;
	return (0);
}

int	readline_pipe(t_lexer *lexer, char *prompt)
{
	char	*append_line;
	char	*join_line;

	append_line = readline(prompt);
	if (!append_line)
		return (1);
	while (append_line && is_empty(append_line))
	{
		free(append_line);
		append_line = readline(prompt);
	}
	if (!append_line)
		return (1);
	join_line = (char *)malloc(sizeof(char) * (ft_strlen(lexer->input) + \
				ft_strlen(append_line) + 2));
	ft_strcpy(join_line, lexer->input);
	ft_strcat(join_line, append_line);
	free(lexer->input);
	free(append_line);
	lexer->input = join_line;
	lexer->input_len = ft_strlen(lexer->input);
	lexer->read_position--;
	lexer->tok_position--;
	return (0);
}

void	do_linebreak(t_lexer *lexer, char *prompt, t_fsm *fsm)
{
	char	*append_line;
	char	*join_line;
	char	*trimmed;

	append_line = readline(prompt);
	if (!append_line)
		return ;
	if (append_line[0] == '\0')
	{
		fsm->input_state = INPUT_COMPLETE;
		free(append_line);
		return ;
	}
	trimmed = ft_strtrim(lexer->input, "\\");
	join_line = (char *)malloc(sizeof(char) * (ft_strlen(lexer->input) + \
				ft_strlen(append_line) + 1));
	ft_strcpy(join_line, trimmed);
	ft_strcat(join_line, append_line);
	free(lexer->input);
	free(trimmed);
	free(append_line);
	lexer->input = join_line;
	lexer->input_len = ft_strlen(lexer->input);
	lexer->read_position -= 2;
	fsm->input_state = N_INPUT;
}
