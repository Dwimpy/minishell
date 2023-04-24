/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var_expansion_matching.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 23:28:42 by arobu             #+#    #+#             */
/*   Updated: 2023/04/24 23:31:14 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "ft_printf.h"
#include "quote_list.h"
#include "fsm.h"

int	is_not_alnum(t_quote *quote, t_index *index)
{
	return (!ft_isalnum(quote->value[index->i]) && \
		quote->value[index->i] != '?' && quote->value[index->i] != '_');
}
