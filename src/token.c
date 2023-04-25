/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 12:28:07 by arobu             #+#    #+#             */
/*   Updated: 2023/04/25 17:02:48 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"

t_token_value	get_value(t_token_type type, \
					char *value, t_redir_type redir_type);
static void		get_operator_value(t_token_value *val, \
					t_token_type type, char *value);
static void		get_operator_great(t_token_value *val, t_redir_type redir_type);
static void		get_operator_dgreat(t_token_value *val, \
					t_redir_type redir_type);

t_token	*new_token(t_token_type type, char *value, t_redir_type redir_type)
{
	t_token			*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	token->value = get_value(type, value, redir_type);
	token->next = NULL;
	return (token);
}

t_token_value	get_value(t_token_type type, \
		char *value, t_redir_type redir_type)
{
	t_token_value	val;

	get_operator_value(&val, type, value);
	if (type == TOKEN_GREAT)
		get_operator_great(&val, redir_type);
	if (type == TOKEN_DLESS)
		val.dless.value = ft_strdup("<<");
	if (type == TOKEN_DGREAT)
		get_operator_dgreat(&val, redir_type);
	if (type == TOKEN_LPARENTHESIS)
		val.lparanthesis.c = '(';
	if (type == TOKEN_RPARENTHESIS)
		val.rparanthesis.c = ')';
	if (type == TOKEN_SEMICOLON)
		val.semicolon.c = ';';
	if (type == TOKEN_NEWLINE)
		return ((t_token_value){.new_line.c = '\n'});
	return (val);
}

static void	get_operator_dgreat(t_token_value *val, t_redir_type redir_type)
{
	if (redir_type == EMPTY)
	{
		(*val).great.symbol = ft_strdup(">>");
		(*val).great.from = redir_type;
	}
	else if (redir_type == STD_IN)
	{
		(*val).great.symbol = ft_strdup("0>>");
		(*val).great.from = redir_type;
	}
	else if (redir_type == STD_OUT)
	{
		(*val).great.symbol = ft_strdup("1>>");
		(*val).great.from = redir_type;
	}
	else if (redir_type == STD_ERR)
	{
		(*val).great.symbol = ft_strdup("2>>");
		(*val).great.from = redir_type;
	}
}

static void	get_operator_value(t_token_value *val, \
	t_token_type type, char *value)
{
	if (type == TOKEN_WORD)
		(*val).word.value = ft_strdup(value);
	if (type == TOKEN_QUOTE)
	{
		(*val).quote.value = ft_strdup(value);
		(*val).quote.quotes = create_list(value);
	}
	if (type == TOKEN_ASSIGN_WORD)
		(*val).assign_word.value = ft_strdup(value);
	if (type == TOKEN_PIPE)
		(*val).pipe.c = '|';
	if (type == TOKEN_AND)
		(*val).and.c = '&';
	if (type == TOKEN_OR_IF)
		(*val).or_if.value = ft_strdup("||");
	if (type == TOKEN_AND_IF)
		(*val).and_if.value = ft_strdup("&&");
	if (type == TOKEN_LESS)
		(*val).less.c = '<';
}

static void	get_operator_great(t_token_value *val, t_redir_type redir_type)
{
	if (redir_type == EMPTY)
	{
		(*val).great.symbol = ft_strdup(">");
		(*val).great.from = redir_type;
	}
	else if (redir_type == STD_IN)
	{
		(*val).great.symbol = ft_strdup("0>");
		(*val).great.from = redir_type;
	}
	else if (redir_type == STD_OUT)
	{
		(*val).great.symbol = ft_strdup("1>");
		(*val).great.from = redir_type;
	}
	else if (redir_type == STD_ERR)
	{
		(*val).great.symbol = ft_strdup("2>");
		(*val).great.from = redir_type;
	}
	return ;
}
