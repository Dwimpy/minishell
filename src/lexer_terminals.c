/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_matching_primitives.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/11 15:17:58 by arobu             #+#    #+#             */
/*   Updated: 2023/03/11 15:21:34 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

int	match_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

int	match_digit(char c)
{
	return (ft_isdigit(c));
}

int	match_symbol(char c)
{
	const char	symbols[35] = "!@#$%^&*()-_+={}[]|\\:;\"',.<>/?`~";
	int			i;

	i = 0;
	while (symbols[i])
	{
		if (c == symbols[i])
			return (1);
		i++;
	}
	return (0);
}

int	match_letter(char c)
{
	return (ft_isalpha(c));
}
