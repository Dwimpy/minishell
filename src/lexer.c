/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/06 16:59:45 by dwimpy            #+#    #+#             */
/*   Updated: 2023/03/23 18:43:15 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include "lexer.h"

void	skip_whitespace(t_lexer *lexer);

void	init_lexer(t_lexer *lexer)
{
	char	*line;
	char	*next_line;
	char	*init_line;
	char	*trimmed;

	line = readline("minishell$ ");
	while (ft_strrchr(line, '\\') != NULL && \
		(ft_strrchr(line, '\\') + 1)[0] == '\0')
	{
		init_line = line;
		trimmed = ft_strtrim(line, "\\\n");
		free(init_line);
		next_line = readline("> ");
		line = ft_strjoin(trimmed, next_line);
		free(trimmed);
		free(next_line);
	}
	lexer->read_position = -1;
	add_history(line);
	lexer->input = ft_strdup(line);
	lexer->input_len = ft_strlen(lexer->input);
	lexer->ch = '\0';
	free(line);
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
	char	buffer[4096];
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
