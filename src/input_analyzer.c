/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_analyzer.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 15:41:35 by arobu             #+#    #+#             */
/*   Updated: 2023/03/31 22:43:19 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

void	analze_quotes(t_lexer *lexer);

int	analyze_input(t_lexer *lexer)
{
	analze_quotes(lexer);
	return (0);
}

void	analze_quotes(t_lexer *lexer)
{
	int			is_single_quote;
	int			is_double_quote;
	char		c;
	char		*new_line;
	char		*prev_line;
	int			init_length;
	int			parens;
	int			parens_counter;;
	HIST_ENTRY	*entry;

	is_single_quote = 0;
	is_double_quote = 0;
	init_length = lexer->input_len;
	parens = 0;
	parens_counter = 1;
	c = get_next_char(lexer);
	while (lexer->read_position != lexer->input_len)
	{
		if ((c == '(' || c == ')') && !is_double_quote && !is_single_quote)
		{
			if (c == ')')
				parens_counter--;
			parens ^= parens_counter;
			if (c == '(')
				parens_counter++;
		}
		if (parens_counter < 0)
		{
			parens = 0;
			break ;
		}
		if (c == '\'' && !is_double_quote)
			is_single_quote ^= 1;
		if (c == '"' && !is_single_quote)
			is_double_quote ^= 1;
		c = get_next_char(lexer);
	}
	while (is_double_quote || is_single_quote || parens)
	{
		new_line = readline(">");
		prev_line = lexer->input;
		lexer->input = ft_strjoin(lexer->input, new_line);
		lexer->input_len += ft_strlen(new_line);
		lexer->read_position--;
		entry = remove_history(history_length - 1);
		free((char *)entry->line);
		free(entry);
		free(prev_line);
		free(new_line);
		add_history(lexer->input);
		while (lexer->input_len != lexer->read_position)
		{
			if ((c == '(' || c == ')') && !is_double_quote && !is_single_quote)
			{
				if (c == ')')
					parens_counter--;
				parens ^= parens_counter;
				if (c == '(')
					parens_counter++;
			}
			if (c == '\'' && !is_double_quote)
				is_single_quote ^= 1;
			if (c == '\"' && !is_single_quote)
				is_double_quote ^= 1;
			c = get_next_char(lexer);
		}
	}
	lexer->read_position = -1;
	return ;
}
