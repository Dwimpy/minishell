/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_helper_funcs.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 16:57:12 by arobu             #+#    #+#             */
/*   Updated: 2023/04/25 17:03:34 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"

static void	print_correct_token_1(t_token *token);
static void	print_correct_token_2(t_token *token);

void	print_token_value(t_token *token)
{
	print_correct_token_1(token);
	print_correct_token_2(token);
}

void	free_token(t_token *token)
{
	if (token->type == TOKEN_WORD)
		free(token->value.word.value);
	if (token->type == TOKEN_QUOTE)
	{
		free_quotelist(token->value.quote.quotes);
		free(token->value.quote.value);
	}
	if (token->type == TOKEN_ASSIGN_WORD)
		free(token->value.word.value);
	if (token->type == TOKEN_OR_IF)
		free(token->value.or_if.value);
	if (token->type == TOKEN_AND_IF)
		free(token->value.and_if.value);
	if (token->type == TOKEN_DLESS)
		free(token->value.dless.value);
	if (token->type == TOKEN_DGREAT)
		free(token->value.dgreat.value);
	if (token->type == TOKEN_GREAT)
		free(token->value.great.symbol);
	if (token->type == TOKEN_EOF)
		free(token->value.dgreat.value);
	free(token);
}

static void	print_correct_token_1(t_token *token)
{
	if (token->type == TOKEN_WORD)
		printf("\t[ %s ] (WORD)\n", token->value.word.value);
	if (token->type == TOKEN_QUOTE)
		printf("\t[ %s ] (QUOTE)\n", token->value.word.value);
	if (token->type == TOKEN_ASSIGN_WORD)
		printf("\t[ %s ] (ASSIGNMENT WORD)\n", token->value.assign_word.value);
	if (token->type == TOKEN_PIPE)
		printf("\t[ %c ] (PIPE)\n", token->value.pipe.c);
	if (token->type == TOKEN_OR_IF)
		printf("\t[ %s ](OR_IF)\n", token->value.or_if.value);
	if (token->type == TOKEN_AND_IF)
		printf("\t[ %s ](AND_IF)\n", token->value.and_if.value);
	if (token->type == TOKEN_AND)
		printf("\t[ %c ] (AND)\n", token->value.and.c);
	if (token->type == TOKEN_LESS)
		printf("\t[ %c ] (LESS)\n", token->value.less.c);
}

static void	print_correct_token_2(t_token *token)
{
	if (token->type == TOKEN_GREAT)
		printf("\t[ %s ] (GREAT)\n", token->value.great.symbol);
	if (token->type == TOKEN_DLESS)
		printf("\t[ %s ] (DLESS)\n", token->value.dless.value);
	if (token->type == TOKEN_DGREAT)
		printf("\t[ %s ] (DGREAT)\n", token->value.dgreat.value);
	if (token->type == TOKEN_LPARENTHESIS)
		printf("\t[ %c ] (L PARENTHESIS)\n", token->value.lparanthesis.c);
	if (token->type == TOKEN_RPARENTHESIS)
		printf("\t[ %c ] (R PARENTHESIS)\n", token->value.rparanthesis.c);
	if (token->type == TOKEN_SEMICOLON)
		printf("\t[ %c ] (SEMICOLON)\n", token->value.semicolon.c);
	if (token->type == TOKEN_NEWLINE)
		printf("\t[ \\n ] (NEWLINE)\n");
	if (token->type == TOKEN_EOF)
		printf("\t[ %s ] (EOF)\n", token->value.dgreat.value);
}
