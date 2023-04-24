/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_list_helperes.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 20:13:59 by arobu             #+#    #+#             */
/*   Updated: 2023/04/24 20:14:07 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token_list.h"

t_token	*peek_token(t_token_list *tokens, int look_ahead_times)
{
	t_token	*token;
	int		i;

	i = 0;
	token = tokens->first;
	while (token && i < look_ahead_times)
	{
		token = token->next;
	}
	return (token);
}

t_token	*get_next_token(t_token *token)
{
	(token) = (token)->next;
	return (token);
}

int	get_nargs(t_token *token)
{
	t_token	*curr_token;
	int		count;

	curr_token = token;
	count = 0;
	while (curr_token->type == TOKEN_WORD)
	{
		count++;
		curr_token = curr_token->next;
	}
	return (count);
}

void	consume_token(t_token_list *tokens)
{
	t_token	*token;

	token = (tokens)->first;
	(tokens)->first = (tokens)->first->next;
	free_token(token);
	tokens->num_tokens--;
	return ;
}

t_token	*get_last_token(t_token_list *token_list)
{
	t_token	*token;

	token = token_list->first;
	if (!token)
		return (NULL);
	while (token->next->type != TOKEN_EOF)
	{
		print_token_value(token);
		token = token->next;
	}
	return (token);
}
