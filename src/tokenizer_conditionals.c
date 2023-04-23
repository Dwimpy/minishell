/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_conditionals.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/23 18:48:55 by arobu             #+#    #+#             */
/*   Updated: 2023/04/23 18:50:18 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"
#include <sys/wait.h>
#include "signals.h"

inline int	is_token_assignment(t_token *token)
{
	return (token->type == TOKEN_ASSIGN_WORD);
}

inline int	is_token_lparen(t_token *token)
{
	return (token->type == TOKEN_LPARENTHESIS);
}

inline int	is_token_rparen(t_token *token)
{
	return (token->type == TOKEN_RPARENTHESIS);
}

inline int	is_token_pipe(t_token *token)
{
	return (token->type == TOKEN_PIPE);
}

inline int	is_token_cmdand(t_token *token)
{
	return (token->type == TOKEN_AND_IF);
}
