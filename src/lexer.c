/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/06 16:59:45 by dwimpy            #+#    #+#             */
/*   Updated: 2023/04/05 15:37:59 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include "lexer.h"

static char	*return_prompt_type(t_incomplete_type type);
static char	*get_new_input_tok_type(t_token_type type);

int	init_lexer(t_lexer *lexer)
{
	char	*history;

	lexer->input = readline("minishell$ ");
	lexer->read_position = 0;
	while (lexer->input[lexer->read_position++] == ' ')
		if (lexer->input[lexer->read_position + 1] == '\0')
			return (1);
	if (!lexer->input || ft_strlen(lexer->input) == 0)
	{
		free(lexer->input);
		return (1);
	}
	history = ft_strdup(lexer->input);
	while (ft_strrchr(lexer->input, '\\') != NULL && \
		(ft_strrchr(lexer->input, '\\') + 1)[0] == '\0')
		if (append_to_input(lexer, NEWLINE, &history))
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
	if (lexer->ch == ';')
		return (tokenize_semicolon(lexer));
	// if (lexer->ch == '\n')
	// 	return (tokenize_newline(lexer));
	return (NULL);
}

char	look_ahead(t_lexer *lexer)
{
	if (lexer->read_position < lexer->input_len)
		return (lexer->input[lexer->read_position + 1]);
	else
		return ('\0');
}

int	append_input_pipe(t_lexer *lexer, t_token_type type)
{
	char	*append_input;
	char	*prev_input;

	append_input = get_new_input_tok_type(type);
	if (!append_input)
		return (1);
	prev_input = lexer->input;
	lexer->input = ft_strjoin(lexer->input, append_input);
	lexer->input_len = ft_strlen(lexer->input);
	free(append_input);
	free(prev_input);
	return (0);
}

int	append_to_input(t_lexer *lexer, t_incomplete_type type, char **curr_history)
{
	char	*append_input;
	char	*prev_input;
	char	*history;

	history = *curr_history;
	append_input = readline(return_prompt_type(type));
	if (!append_input)
		return (1);
	prev_input = lexer->input;
	if (append_input[0] == '\0')
	{
		(*curr_history) = str_join_newline(*curr_history, "\n");
		free(history);
		free(append_input);
		return (1);
	}
	lexer->input = ft_strjoin(lexer->input, append_input);
	(*curr_history) = str_join_newline(*curr_history, append_input);
	free(history);
	free(append_input);
	free(prev_input);
	return (0);
}

int	append_to_input_special(t_lexer *lexer, \
		t_incomplete_type type, char **curr_history)
{
	char	*append_input;
	char	*prev_input;

	if (!*curr_history)
		(*curr_history) = ft_strdup(lexer->input);
	prev_input = lexer->input;
	append_input = get_new_input(type);
	lexer->read_position--;
	lexer->input = str_join_newline(lexer->input, append_input);
	get_new_history(append_input, type, curr_history);
	free(append_input);
	free(prev_input);
	return (ft_strlen(lexer->input));

}

void	get_new_history(char *append_input, t_incomplete_type type, \
							char **curr_history)
{
	char	*history;

	if (*curr_history && (type == SQUOTE || type == DQUOTE))
	{
		history = (*curr_history);
		(*curr_history) = str_join_newline(*curr_history, append_input);
		free(history);
	}
	else if (*curr_history && type == SUBSH)
	{
		history = (*curr_history);
		(*curr_history) = str_join_newline(*curr_history, append_input);
		free(history);
	}
}

char	*get_new_input(t_incomplete_type type)
{
	if (type == NEWLINE)
		return (readline(return_prompt_type(type)));
	else if (type == SQUOTE)
		return (readline(return_prompt_type(type)));
	else if (type == DQUOTE)
		return (readline(return_prompt_type(type)));
	else if (type == SUBSH)
		return (readline(return_prompt_type(type)));
	return (NULL);
}

static char	*get_new_input_tok_type(t_token_type type)
{
	if (type == TOKEN_PIPE)
		return (readline("pipe> "));
	else if (type == TOKEN_AND_IF)
		return (readline("cmdand> "));
	else if (type == TOKEN_OR_IF)
		return (readline("cmdor> "));
	return (NULL);
}

static char	*return_prompt_type(t_incomplete_type type)
{
	if (type == NEWLINE)
		return ("> ");
	else if (type == SQUOTE)
		return ("squote> ");
	else if (type == DQUOTE)
		return ("dquote> ");
	else if (type == SUBSH)
		return ("subsh> ");
	return (NULL);
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
