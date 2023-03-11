/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_matching.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 14:12:41 by arobu             #+#    #+#             */
/*   Updated: 2023/03/11 15:19:20 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

int	match_alphanumeric(char c)
{
	return (match_character(c) || match_digit(c));
}

int	match_character(char c)
{
	return (match_letter(c) || match_digit(c) || match_symbol(c));
}

int	match_word(char c)
{
	return (ft_isalnum(c) || c == '_');
}
