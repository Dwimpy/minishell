/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var_expansion.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 23:12:31 by arobu             #+#    #+#             */
/*   Updated: 2023/04/24 23:43:27 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "ft_printf.h"
#include "quote_list.h"
#include "fsm.h"

static void	in_expand_var_dollar(t_arglist *arglist, \
			t_quote *quote, t_index *index, t_fsm *fsm);
static void	in_expand_special(t_index *index, t_fsm *fsm);
static void	fsm_in_not_alnum(t_arglist *arglist, t_quote *quote, \
	t_fsm *fsm, t_index *index);
static void	fsm_expand_has_ended(t_arglist *arglist, t_quote *quote, \
			t_index *index, t_fsm *fsm);

void	do_fsm_in_expand_var(t_quote *quote, t_index *index, \
			t_arglist *arglist, t_fsm *fsm)
{
	if (quote->value[index->i] == '$')
		in_expand_var_dollar(arglist, quote, index, fsm);
	else if (fsm->input_state == REGULAR && quote->value[index->i] == '\'')
		in_expand_special(index, fsm);
	else if (is_not_alnum(quote, index))
		fsm_in_not_alnum(arglist, quote, fsm, index);
	else if (quote->value[index->i + 1] == '\0')
		fsm_expand_has_ended(arglist, quote, index, fsm);
}

static void	fsm_expand_has_ended(t_arglist *arglist, t_quote *quote, \
				t_index *index, t_fsm *fsm)
{
	new_argument(arglist, create_expand_arg(&quote->value[index->start], \
		0, index->i - index->start + 1, EXPAND));
	if (fsm->input_state == REGULAR)
		arglist->last->expand_type = 0;
	else if (fsm->input_state == IN_DQUOTE)
		arglist->last->expand_type = 1;
	fsm->expand_var = NOT_IN_EXPAND_VAR;
	index->start = index->i + 1;
}

static void	fsm_in_not_alnum(t_arglist *arglist, t_quote *quote, \
	t_fsm *fsm, t_index *index)
{
	new_argument(arglist, create_expand_arg(&quote->value[index->start], \
			0, index->i - index->start, EXPAND));
	if (fsm->input_state == REGULAR)
		arglist->last->expand_type = 0;
	else if (fsm->input_state == IN_DQUOTE)
		arglist->last->expand_type = 1;
	fsm->expand_var = NOT_IN_EXPAND_VAR;
	index->start = index->i;
}

static void	in_expand_special(t_index *index, t_fsm *fsm)
{
	fsm->expand_var = NOT_IN_EXPAND_VAR;
	fsm->input_state = IN_SQUOTE;
	index->start = index->i + 1;
}

static void	in_expand_var_dollar(t_arglist *arglist, \
			t_quote *quote, t_index *index, t_fsm *fsm)
{
	new_argument(arglist, create_expand_arg(&quote->value[index->start], \
		0, index->i - index->start, EXPAND));
	if (fsm->input_state == REGULAR)
		arglist->last->expand_type = 0;
	else if (fsm->input_state == IN_DQUOTE)
		arglist->last->expand_type = 1;
	fsm->expand_var = NOT_IN_EXPAND_VAR;
	index->start = index->i + 1;
	index->i--;
}
