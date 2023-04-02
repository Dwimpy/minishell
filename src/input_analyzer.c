/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_analyzer.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 15:41:35 by arobu             #+#    #+#             */
/*   Updated: 2023/04/02 14:16:30 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

void		analze_quotes(t_lexer *lexer);
static int	is_incomplete_input_loop(t_control control);
static int	analyze_incomplete_input(t_control *control, char c);
static void	update_history(t_control control);
static void	remove_entry(t_control control);

int	analyze_input(t_lexer *lexer)
{
	analze_quotes(lexer);
	return (0);
}

void	analze_quotes(t_lexer *lexer)
{
	t_control	control;
	char		c;

	control = init_control_structure();
	c = get_next_char(lexer);
	while (lexer->read_position != lexer->input_len)
	{
		if (analyze_incomplete_input(&control, c))
			break ;
		c = get_next_char(lexer);
	}
	while (is_incomplete_input_loop(control))
	{
		remove_entry(control);
		lexer->input_len = get_new_lexer_input(lexer, &control);
		while (lexer->input_len != lexer->read_position)
		{
			handle_incomplete_input(&control, c);
			c = get_next_char(lexer);
		}
	}
	update_history(control);
	lexer->read_position = -1;
	return ;
}

static void	remove_entry(t_control control)
{
	HIST_ENTRY	*entry;

	if (!control.history)
	{
		entry = remove_history(history_length - 1);
		free((char *)entry->line);
		free(entry);
	}
}

static void	update_history(t_control control)
{
	if (control.history)
	{
		add_history(control.history);
		free(control.history);
	}
}

int	get_new_lexer_input(t_lexer *lexer, t_control *control)
{
	if (control->is_single_quote)
		return (append_to_input_special(lexer, SQUOTE, &control->history));
	else if (control->is_double_quote)
		return (append_to_input_special(lexer, DQUOTE, &control->history));
	else if (control->parens)
		return (append_to_input_special(lexer, SUBSH, &control->history));
	return (0);
}

static int	is_incomplete_input_loop(t_control control)
{
	return (control.is_double_quote || \
				control.is_single_quote || \
					control.parens);
}

void	handle_incomplete_input(t_control *control, char c)
{
	if (is_incomplete_input(*control, c))
	{
		if (c == ')')
			control->parens_counter--;
		control->parens ^= control->parens_counter;
		if (c == '(')
			control->parens_counter++;
	}
	if (c == '\'' && !control->is_double_quote)
		control->is_single_quote ^= 1;
	if (c == '\"' && !control->is_single_quote)
		control->is_double_quote ^= 1;
}

static int	analyze_incomplete_input(t_control *control, char c)
{
	if (is_incomplete_input(*control, c))
	{
		if (c == ')')
			control->parens_counter--;
		control->parens ^= control->parens_counter;
		if (c == '(')
			control->parens_counter++;
	}
	if (control->parens_counter < 0)
	{
		control->parens = 0;
		return (1);
	}
	if (c == '\'' && !control->is_double_quote)
		control->is_single_quote ^= 1;
	if (c == '"' && !control->is_single_quote)
		control->is_double_quote ^= 1;
	return (0);
}

int	is_incomplete_input(t_control control, char c)
{
	return ((c == '(' || c == ')') && \
		!control.is_single_quote && !control.is_single_quote);
}

t_control	init_control_structure(void)
{
	t_control	control;

	control.is_double_quote = 0;
	control.is_single_quote = 0;
	control.parens = 0;
	control.parens_counter = 1;
	control.history = NULL;
	return (control);
}
