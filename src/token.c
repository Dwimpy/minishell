/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 12:28:07 by arobu             #+#    #+#             */
/*   Updated: 2023/03/11 17:28:11 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"

t_token	*new_token(t_token_type type, t_token_value value)
{
	t_token			*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	token->value = value;
	token->next = NULL;
	return (token);
}

t_token	*new_empty_token(void)
{
	t_token	*token;

	token = new_token(TOKEN_EOF, (t_token_value){.operator = '\0'});
	return (token);
}

void	print_token_value(t_token *token)
{
	printf("Type: %d\t", token->type);
	if (is_string_type(token->type))
		printf("Token value: [%s]\n", token->value.string);
	else if (is_operator_type(token->type))
		printf("Token value: [%c]\n", token->value.operator);
}

void	free_token(t_token *token)
{
	if (is_string_type(token->type))
	{
		free(token->value.string);
	}
	free(token);
}
