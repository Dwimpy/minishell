/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_analyzer_is_token.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 17:36:48 by arobu             #+#    #+#             */
/*   Updated: 2023/03/31 17:36:55 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"

int	is_pipe(t_token *token)
{
	return (token->type == TOKEN_PIPE);
}

int	is_logical_op(t_token *token)
{
	return (token->type == TOKEN_AND_IF || token->type == TOKEN_OR_IF);
}

int	is_left_parenthesis(t_token *token)
{
	return (token->type == TOKEN_LPARENTHESIS);
}

int	is_right_parenthesis(t_token *token)
{
	return (token->type == TOKEN_RPARENTHESIS);
}

int	is_token_eof(t_token *token)
{
	return (token->type == TOKEN_EOF);
}
