/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_cond.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/11 15:27:36 by arobu             #+#    #+#             */
/*   Updated: 2023/03/11 21:23:16 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"

int	is_string_type(t_token_type type)
{
	return (type == TOKEN_WORD || type == TOKEN_D_AND || type == TOKEN_OR || \
		type == TOKEN_REDIR_INPUT_APPEND || type == TOKEN_REDIR_OUTPUT_APPEND);
}

int	is_operator_type(t_token_type type)
{
	return (type == TOKEN_PIPE || type == TOKEN_AND || type == TOKEN_EOF || \
		type == TOKEN_REDIR_INPUT || type == TOKEN_REDIR_OUTPUT);
}
