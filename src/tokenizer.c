/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/06 19:38:03 by arobu             #+#    #+#             */
/*   Updated: 2023/04/06 22:11:43 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"

int	gen_input(t_input *input)
{
	t_fsm	fsm;

	fsm.state = GET_INPUT;
	fsm.input_state = N_INPUT;
	fsm.tok_state = N_TOKENIZER;
	ft_putstr_fd((char *)hashmap_get(input->hashmap, "PWD"), 1);
	input->lexer.input = readline(" >$ ");
	input->lexer.input_len = ft_strlen(input->lexer.input);
	input->lexer.read_position = -1;
	while (fsm.state != COMPLETE && fsm.state != ERROR)
	{
		get_the_input(input, &fsm);
	}
	printf("%s\n", input->lexer.input);
	return (0);
}

void	get_the_input(t_input *input, t_fsm *fsm)
{
	t_lexer		*lexer;
	char		c;

	lexer = &input->lexer;
	while (fsm->input_state != INPUT_COMPLETE)
	{
		c = get_next_char(lexer);
		if (fsm->input_state == N_INPUT)
		{
			if (c == '\0' && !input->lexer.input_len)
			{
				fsm->input_state = INPUT_COMPLETE;
				fsm->state = COMPLETE;
			}
			else if (c == '\0')
				fsm->input_state = INPUT_COMPLETE;
			if (c == '\\' && look_ahead(lexer) == '\0')
				fsm->input_state = IN_LINEBR;
			if (c == '\'')
				fsm->input_state = IN_SQUOTE;
			if (c == '"')
				fsm->input_state = IN_DQUOTE;
		}
		else if (fsm->input_state == IN_LINEBR)
			do_linebreak(lexer, "> ", fsm);
		else if (fsm->input_state == IN_SQUOTE)
			do_squote(lexer, fsm);
		else if (fsm->input_state == IN_DQUOTE)
			do_dquote(lexer, fsm);
	}
	printf("State: %d\t", fsm->state);
	printf("State: %d\n", fsm->input_state);
	add_history(lexer->input);
	fsm->state = COMPLETE;
}

void	do_linebreak(t_lexer *lexer, char *prompt, t_fsm *fsm)
{
	char	*append_line;
	char	*join_line;

	append_line = readline(prompt);
	if (!append_line)
		return ;
	join_line = (char *)malloc(sizeof(char) * (ft_strlen(lexer->input) + \
				ft_strlen(append_line) + 1));
	ft_strcpy(join_line, lexer->input);
	ft_strcat(join_line, append_line);
	free(lexer->input);
	free(append_line);
	lexer->input = join_line;
	lexer->input_len = ft_strlen(lexer->input);
	lexer->read_position--;
	fsm->input_state = N_INPUT;
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
