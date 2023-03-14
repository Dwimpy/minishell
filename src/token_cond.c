/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_cond.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/11 15:27:36 by arobu             #+#    #+#             */
/*   Updated: 2023/03/14 12:05:14 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"

int	is_string_type(t_token_type type)
{
	return (type == TOKEN_WORD || type == TOKEN_AND_IF || type == TOKEN_OR_IF || \
		type == TOKEN_DLESS || type == TOKEN_DGREAT);
}

int	is_operator_type(t_token_type type)
{
	return (type == TOKEN_PIPE || type == TOKEN_AND || \
		type == TOKEN_DLESS || type == TOKEN_DGREAT);
}
