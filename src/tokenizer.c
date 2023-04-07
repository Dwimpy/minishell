/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/06 19:38:03 by arobu             #+#    #+#             */
/*   Updated: 2023/04/07 19:38:31 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"

int	gen_input(t_input *input)
{
	t_fsm	fsm;
	char	*prompt;

	fsm.state = GET_INPUT;
	fsm.input_state = N_INPUT;
	fsm.tok_state = N_TOKENIZER;
	input->lexer.input = read_from_stdin();
	input->lexer.input_len = ft_strlen(input->lexer.input);
	input->lexer.read_position = -1;
	input->lexer.tok_position = -1;
	while (fsm.state != COMPLETE && fsm.state != ERROR)
	{
		get_the_input(input, &fsm);
		tokenize(input, &fsm);
		printf("State: [%d]\t Substate: [%d]\n", fsm.state, fsm.tok_state);
	}
	// printf("%s\n", input->lexer.input);
	return (0);
}

void	tokenize(t_input *input, t_fsm *fsm)
{
	t_lexer	*lexer;
	t_token	*token;

	lexer = &input->lexer;
	fsm->state = GET_TOKENS;
	token = NULL;
	while (fsm->tok_state != TOK_COMPLETE && \
			fsm->tok_state != INCOMPLETE && fsm->state != ERROR)
	{
		token = create_next_token(lexer);
		if (!token)
		{
			if (input->tokens->last->type != TOKEN_WORD && \
				input->tokens->last->type != TOKEN_SEMICOLON && \
				input->tokens->last->type != TOKEN_SQUOTE && \
				input->tokens->last->type != TOKEN_DQUOTE && \
				input->tokens->last->type != TOKEN_RPARENTHESIS)
			{
				fsm->state = INCOMPLETE;
				if (input->tokens->last->type == TOKEN_PIPE)
					fsm->tok_state = TOK_PIPE;
				else if (input->tokens->last->type == TOKEN_AND_IF)
					fsm->tok_state = TOK_AND_IF;
				else if (input->tokens->last->type == TOKEN_OR_IF)
					fsm->tok_state = TOK_OR_IF;
				return ;
			}
			else
			{
				fsm->tok_state = TOK_COMPLETE;
				fsm->state = COMPLETE;
				print_tokens(input->tokens);
			}
		}
		else
		{
			
		}
		add_token(input->tokens, token);
	}
}

void	get_the_input(t_input *input, t_fsm *fsm)
{
	t_lexer		*lexer;
	char		c;

	lexer = &input->lexer;
	fsm->input_state = N_INPUT;
	while (fsm->input_state != INPUT_COMPLETE)
	{
		if (fsm->state == GET_INPUT)
		{
			c = get_next_char(lexer);
			if (fsm->input_state == N_INPUT)
				fsm_input_state_update(c, lexer, fsm);
			else if (fsm->input_state == IN_LINEBR)
				do_linebreak(lexer, "> ", fsm);
			else if (fsm->input_state == IN_SQUOTE)
				do_squote(lexer, fsm);
			else if (fsm->input_state == IN_DQUOTE)
				do_dquote(lexer, fsm);
			else if (fsm->input_state == IN_SUBSH)
				do_subsh(lexer, fsm);
		}
		else if (fsm->state == INCOMPLETE)
		{
			if (fsm->tok_state == TOK_PIPE)
				do_in_pipe(lexer, fsm);
			else if (fsm->tok_state == TOK_AND_IF)
				do_in_cmdand(lexer, fsm);
			else if (fsm->tok_state == TOK_OR_IF)
				do_in_cmdor(lexer, fsm);
		}
	}
	// printf("State: %d\t", fsm->state);
	// printf("State: %d\n", fsm->input_state);
	add_history(lexer->input);
}

char	*get_prompt_dir(void)
{
	char	path[264];

	if (getcwd(path, sizeof(path)))
	{
		ft_strcat(path, ">$ ");
		return (ft_strdup(path));
	}
	else
		return (ft_strdup(">$ "));
}

char	*read_from_stdin(void)
{
	char	*prompt;
	char	*line;

	prompt = get_prompt_dir();
	line = readline(prompt);
	free(prompt);
	return (line);
}


void	fsm_input_state_update(char c, t_lexer *lexer, t_fsm *fsm)
{
	if (c == '\0' && !lexer->input_len)
	{
		fsm->input_state = INPUT_COMPLETE;
		fsm->state = COMPLETE;
	}
	else if (c == '\0')
	{
		fsm->input_state = INPUT_COMPLETE;
	}
	if (c == '\\' && look_ahead(lexer) == '\0')
		fsm->input_state = IN_LINEBR;
	if (c == '\'')
		fsm->input_state = IN_SQUOTE;
	if (c == '"')
		fsm->input_state = IN_DQUOTE;
	if (c == '(')
		fsm->input_state = IN_SUBSH;
}

void	do_linebreak(t_lexer *lexer, char *prompt, t_fsm *fsm)
{
	char	*append_line;
	char	*join_line;
	char	*trimmed;

	append_line = readline(prompt);
	if (!append_line)
		return ;
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
	lexer->read_position--;
	fsm->input_state = N_INPUT;
}

void	do_in_pipe(t_lexer *lexer, t_fsm *fsm)
{
	readline_pipe(lexer, "pipe> ", fsm);
	fsm->state = GET_INPUT;
	fsm->input_state = N_INPUT;
	fsm->tok_state = N_TOKENIZER;
}

void	do_in_cmdand(t_lexer *lexer, t_fsm *fsm)
{
	readline_pipe(lexer, "cmdand> ", fsm);
	fsm->state = GET_INPUT;
	fsm->input_state = N_INPUT;
	fsm->tok_state = N_TOKENIZER;
}

void	do_in_cmdor(t_lexer *lexer, t_fsm *fsm)
{
	readline_pipe(lexer, "cmdor> ", fsm);
	fsm->state = GET_INPUT;
	fsm->input_state = N_INPUT;
	fsm->tok_state = N_TOKENIZER;
}

int	is_empty(char *str)
{
	int	i;

	i = 0;
	while (match_whitespace(str[i]))
		i++;
	if (str[i] == '\0')
		return (1);
	return (0);
}

void	do_squote(t_lexer *lexer, t_fsm *fsm)
{
	if (lexer->ch == '\0')
		readline_new_line(lexer, "squote> ", fsm);
	else if (lexer->ch == '\'')
		fsm->input_state = N_INPUT;
}

void	do_dquote(t_lexer *lexer, t_fsm *fsm)
{
	if (lexer->ch == '\0')
		readline_new_line(lexer, "dquote> ", fsm);
	else if (lexer->ch == '\"')
		fsm->input_state = N_INPUT;
}

void	do_subsh(t_lexer *lexer, t_fsm *fsm)
{
	if (lexer->ch == '\0')
		readline_no_new_line(lexer, "subsh> ", fsm);
	else if (lexer->ch == ')')
		fsm->input_state = N_INPUT;
}

void	readline_new_line(t_lexer *lexer, char *prompt, t_fsm *fsm)
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

void	readline_no_new_line(t_lexer *lexer, char *prompt, t_fsm *fsm)
{
	char	*append_line;
	char	*join_line;

	append_line = readline(prompt);
	if (!append_line)
		return ;
	join_line = (char *)malloc(sizeof(char) * (ft_strlen(lexer->input) + \
				ft_strlen(append_line) + 2));
	ft_strcpy(join_line, lexer->input);
	ft_strcat(join_line, append_line);
	free(lexer->input);
	free(append_line);
	lexer->input = join_line;
	lexer->input_len = ft_strlen(lexer->input);
	lexer->read_position--;
}

void	readline_pipe(t_lexer *lexer, char *prompt, t_fsm *fsm)
{
	char	*append_line;
	char	*join_line;

	append_line = readline(prompt);
	while (is_empty(append_line))
	{
		free(append_line);
		append_line = readline(prompt);
	}
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
}
