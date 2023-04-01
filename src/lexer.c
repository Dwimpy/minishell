/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/06 16:59:45 by dwimpy            #+#    #+#             */
/*   Updated: 2023/04/01 17:46:19 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include "lexer.h"
#include <string.h>

void	skip_whitespace(t_lexer *lexer);

int	init_lexer(t_lexer *lexer)
{
	char	*history;

	lexer->input = readline("minishell$ ");
	if (!lexer->input)
		return (1);
	history = ft_strdup(lexer->input);
	while (ft_strrchr(lexer->input, '\\') != NULL && \
		(ft_strrchr(lexer->input, '\\') + 1)[0] == '\0')
		if (append_to_input(lexer, "> ", &history))
			break ;
	lexer->read_position = -1;
	lexer->input_len = ft_strlen(lexer->input);
	add_history(history);
	free(history);
	lexer->ch = '\0';
	return (0);
}

char	get_next_char(t_lexer *lexer)
{
	lexer->read_position++;
	if (lexer->read_position >= lexer->input_len)
	{
		lexer->ch = '\0';
		return (lexer->ch);
	}
	else
		lexer->ch = lexer->input[lexer->read_position];
	return (lexer->ch);
}

t_token	*create_next_token(t_lexer *lexer)
{
	int		i;

	i = 0;
	get_next_char(lexer);
	if (lexer->ch == '\0')
		return (new_token(TOKEN_EOF, NULL));
	if (match_word(lexer->ch))
		return (tokenize_word(lexer));
	if (lexer->ch == '|')
		return (tokenize_pipe(lexer));
	if (lexer->ch == '&')
		return (tokenize_ampersand(lexer));
	if (lexer->ch == '<')
		return (tokenize_redir_input(lexer));
	if (lexer->ch == '>')
		return (tokenize_redir_output(lexer));
	if (lexer->ch == '(' || lexer->ch == ')')
		return (tokenize_braces(lexer));
	return (NULL);
}

char	look_ahead(t_lexer *lexer)
{
	if (lexer->read_position < lexer->input_len)
		return (lexer->input[lexer->read_position + 1]);
	else
		return ('\0');
}

int	append_to_input(t_lexer *lexer, char *prompt, char **curr_history)
{
	char	*append_input;
	char	*prev_input;
	char	*history;

	history = *curr_history;
	append_input = readline(prompt);
	if (!append_input)
		return (1);
	prev_input = lexer->input;
	if (append_input[0] == '\0')
	{
		(*curr_history) = str_join_newline(*curr_history, "\n");
		free(append_input);
		return (1);
	}
	(*curr_history) = str_join_newline(*curr_history, append_input);
	lexer->input = ft_strjoin(lexer->input, append_input);
	free(history);
	free(append_input);
	free(prev_input);
	return (0);
}

char	*str_join_newline(char *s1, char *s2)
{
	char	*join_newline;

	if (!s1 || !s2)
		return (NULL);
	join_newline = (char *)malloc(sizeof(char) * \
		(ft_strlen(s1) + ft_strlen(s2) + 2));
	if (!join_newline)
		return (NULL);
	ft_strcpy(join_newline, s1);
	ft_strcat(join_newline, "\n");
	ft_strcat(join_newline, s2);
	return (join_newline);
}

void	history_append_with_new_line(t_lexer *lexer, \
			char **curr_history)
{
	char	*history;
	int		i;

	history = *curr_history;
	i = 0;
	*curr_history = ft_strjoin(lexer->input, *curr_history);
	if (history)
		free(history);
	while ((*curr_history)[i] != '\\')
		i++;
	(*curr_history)[i] = '\n';
}
