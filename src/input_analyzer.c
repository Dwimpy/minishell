/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_analyzer.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 15:41:35 by arobu             #+#    #+#             */
/*   Updated: 2023/03/30 01:36:00 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

void	analze_quotes(t_lexer *lexer);

int	analyze_input(t_lexer *lexer)
{
	analze_quotes(lexer);
}

void	analze_quotes(t_lexer *lexer)
{
	int		is_single_quote;
	int		is_double_quote;
	char	c;
	char	*new_line;
	int		init_length;

	is_single_quote = 0;
	is_double_quote = 0;
	c = get_next_char(lexer);
	init_length = lexer->input_len;
	while (lexer->read_position != lexer->input_len)
	{
		if (c == '\'' && !is_double_quote)
			is_single_quote ^= 1;
		if (c == '"' && !is_single_quote)
			is_double_quote ^= 1;
		c = get_next_char(lexer);
	}
	while (is_double_quote || is_single_quote)
	{
		printf("WTF");
		new_line = readline(">");
		lexer->input = ft_strtrim(ft_strjoin(lexer->input, new_line), "\n");
		lexer->input_len += ft_strlen(new_line);
		lexer->read_position--;
		while (lexer->input_len != lexer->read_position)
		{
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
