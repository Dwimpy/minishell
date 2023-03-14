/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 12:28:07 by arobu             #+#    #+#             */
/*   Updated: 2023/03/14 17:02:20 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"

t_token_value	get_value(t_token_type type, char *value);

t_token	*new_token(t_token_type type, char *value)
{
	t_token			*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	token->value = get_value(type, value);
	token->next = NULL;
	return (token);
}

t_token_value	get_value(t_token_type type, char *value)
{
	if (type == TOKEN_WORD)
		return ((t_token_value){.word.value = ft_strdup(value)});
	if (type == TOKEN_PIPE)
		return ((t_token_value){.pipe.c = '|'});
	if (type == TOKEN_AND)
		return ((t_token_value){.and.c = '&'});
	if (type == TOKEN_OR_IF)
		return ((t_token_value){.or_if.value = ft_strdup("||")});
	if (type == TOKEN_AND_IF)
		return ((t_token_value){.and_if.value = ft_strdup("&&")});
	if (type == TOKEN_LESS)
		return ((t_token_value){.less.c = '<'});
	if (type == TOKEN_GREAT)
		return ((t_token_value){.great.c = '>'});
	if (type == TOKEN_DLESS)
		return ((t_token_value){.dless.value = ft_strdup("<<")});
	if (type == TOKEN_DGREAT)
		return ((t_token_value){.dgreat.value = ft_strdup(">>")});
	return ((t_token_value){.eof = '\0'});
}

void	print_token_value(t_token *token)
{
	if (token->type == TOKEN_WORD)
		printf("\t[ %s ] (WORD)\n", token->value.word.value);
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
	if (token->type == TOKEN_GREAT)
		printf("\t[ %c ] (GREAT)\n", token->value.great.c);
	if (token->type == TOKEN_DLESS)
		printf("\t[ %s ] (DLESS)\n", token->value.dless.value);
	if (token->type == TOKEN_DGREAT)
		printf("\t[ %s ] (DGREAT)\n", token->value.dgreat.value);
	if (token->type == TOKEN_EOF)
		printf("\t[ %s ] (DGREAT)\n", token->value.dgreat.value);

}

void	free_token(t_token *token)
{
	if (token->type == TOKEN_WORD)
		free(token->value.word.value);
	if (token->type == TOKEN_OR_IF)
		free(token->value.or_if.value);
	if (token->type == TOKEN_AND_IF)
		free(token->value.and_if.value);
	if (token->type == TOKEN_DLESS)
		free(token->value.dless.value);
	if (token->type == TOKEN_DGREAT)
		free(token->value.dgreat.value);
	if (token->type == TOKEN_EOF)
		free(token->value.dgreat.value);
	free(token);
}
