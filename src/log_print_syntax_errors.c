/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log_print_syntax_errors.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 21:49:59 by arobu             #+#    #+#             */
/*   Updated: 2023/04/24 21:52:38 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "ft_printf.h"
#include "quote_list.h"
#include "fsm.h"

static void	print_error(int unexpected);

void	print_syntax_error(int unexpected)
{
	if (unexpected != TOKEN_END_OF_FILE)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
		print_error(unexpected);
	}
	else
		ft_putstr_fd("minishell: syntax error: unexpected end of file\n", 2);
}

static void	print_error(int unexpected)
{
	if (unexpected == TOKEN_EOF)
		ft_putstr_fd("newline'\n", 2);
	else if (unexpected == TOKEN_PIPE)
		ft_putstr_fd("|'\n", 2);
	else if (unexpected == TOKEN_LESS)
		ft_putstr_fd("<'\n", 2);
	else if (unexpected == TOKEN_GREAT)
		ft_putstr_fd(">'\n", 2);
	else if (unexpected == TOKEN_DLESS)
		ft_putstr_fd("<<'\n", 2);
	else if (unexpected == TOKEN_DGREAT)
		ft_putstr_fd(">>'\n", 2);
	else if (unexpected == TOKEN_AND)
		ft_putstr_fd("&'\n", 2);
	else if (unexpected == TOKEN_AND_IF)
		ft_putstr_fd("&&'\n", 2);
	else if (unexpected == TOKEN_OR_IF)
		ft_putstr_fd("||'\n", 2);
	else if (unexpected == TOKEN_RPARENTHESIS)
		ft_putstr_fd(")'\n", 2);
	else if (unexpected == TOKEN_LPARENTHESIS)
		ft_putstr_fd("('\n", 2);
	else if (unexpected == TOKEN_SEMICOLON)
		ft_putstr_fd(";'\n", 2);
}
