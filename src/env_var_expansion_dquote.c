/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var_expansion_dquote.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 23:52:41 by arobu             #+#    #+#             */
/*   Updated: 2023/04/24 23:55:42 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "ft_printf.h"
#include "quote_list.h"
#include "fsm.h"

void	fsm_env_var_dquote(t_arglist *arglist, t_quote *quote, \
		t_index *index, t_fsm *fsm)
{
	if (quote->value[index->i] == '$')
	{
		if (index->start < index->i)
			new_argument(arglist, create_expand_arg(&quote->value \
				[index->start], 0, index->i - index->start, NON_EXPAND));
		fsm->expand_var = IN_EXPAND_VAR;
		index->start = index->i;
	}
	else if (quote->value[index->i] == '\"')
	{
		new_argument(arglist, create_expand_arg(&quote->value[index->start], \
			0, index->i - index->start, NON_EXPAND));
		fsm->input_state = REGULAR;
		index->start = index->i + 1;
	}
	else if (!ft_isalnum(quote->value[index->i]))
	{
		new_argument(arglist, create_expand_arg(&quote->value[index->start], \
		0, index->i - index->start + 1, NON_EXPAND));
		index->start = index->i + 1;
	}
}
