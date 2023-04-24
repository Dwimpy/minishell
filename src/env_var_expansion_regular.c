/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var_expansion_regular.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 23:58:13 by arobu             #+#    #+#             */
/*   Updated: 2023/04/25 00:07:21 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "ft_printf.h"
#include "quote_list.h"
#include "fsm.h"

void	fsm_env_var_regular(t_arglist *arglist, \
			t_quote *quote, t_index *index, t_fsm *fsm)
{
	if (quote->value[index->i] == '$')
	{
		if (index->start < index->i)
			new_argument(arglist, create_expand_arg(&quote->value \
				[index->start], 0, index->i - index->start, NON_EXPAND));
		fsm->expand_var = IN_EXPAND_VAR;
		index->start = index->i;
	}
	else if (quote->value[index->i] == '\'')
		fsm->input_state = IN_SQUOTE;
	else if (quote->value[index->i] == '\"')
		fsm->input_state = IN_DQUOTE;
	else if (!ft_isalnum(quote->value[index->i]))
	{
		new_argument(arglist, create_expand_arg(&quote->value \
			[index->start], 0, index->i - index->start + 1, NON_EXPAND));
		index->start = index->i + 1;
	}
	else if (quote->value[index->i + 1] == '\0')
	{
		new_argument(arglist, create_expand_arg(&quote->value \
			[index->start], 0, index->i - index->start + 1, NON_EXPAND));
		index->start = index->i + 1;
	}
}
