/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var_expansions.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 21:15:48 by arobu             #+#    #+#             */
/*   Updated: 2023/04/25 00:06:39 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "ft_printf.h"
#include "quote_list.h"
#include "fsm.h"

int		has_ended(t_quote *quote, t_index *index, t_arglist *arglist);
void	do_fsm_state_n_input(t_fsm *fsm, t_quote *quote, t_index *index);
void	do_fsm_special_case(t_arglist *arglist, \
			t_quote *quote, t_index *index);
void	do_fsm_in_expand_var(t_quote *quote, t_index *index, \
			t_arglist *arglist, t_fsm *fsm);
void	fsm_env_var_squote(t_arglist *arglist, t_quote *quote, \
			t_index *index, t_fsm *fsm);
void	fsm_env_var_dquote(t_arglist *arglist, t_quote *quote, \
		t_index *index, t_fsm *fsm);
void	fsm_env_var_regular(t_arglist *arglist, \
			t_quote *quote, t_index *index, t_fsm *fsm);
void	run_fsm_env_explansion(t_arglist *arglist, t_quote *quote, \
			t_index *index, t_fsm *fsm);

t_arglist	*expand_vars(char	*value)
{
	t_quotelist	*quotelist;
	t_quote		*quote;
	t_arglist	*arglist;
	t_fsm		fsm;
	t_index		index;

	if (!value)
		return (NULL);
	quotelist = create_list(value);
	arglist = new_arglist();
	quote = quotelist->first;
	fsm.input_state = N_INPUT;
	fsm.expand_var = NOT_IN_EXPAND_VAR;
	while (quote)
	{
		fsm.input_state = N_INPUT;
		index.i = 0;
		index.start = 0;
		while (fsm.input_state != INPUT_COMPLETE)
			run_fsm_env_explansion(arglist, quote, &index, &fsm);
		quote = quote->next;
	}
	free_quotelist(quotelist);
	return (arglist);
}

void	run_fsm_env_explansion(t_arglist *arglist, t_quote *quote, \
			t_index *index, t_fsm *fsm)
{
	if (has_ended(quote, index, arglist))
		fsm->input_state = INPUT_COMPLETE;
	if (fsm->input_state == N_INPUT)
		do_fsm_state_n_input(fsm, quote, index);
	if (is_special_case(quote, fsm, index->i))
		do_fsm_special_case(arglist, quote, index);
	else if (fsm->expand_var == IN_EXPAND_VAR)
		do_fsm_in_expand_var(quote, index, arglist, fsm);
	else if (fsm->input_state == IN_SQUOTE)
		fsm_env_var_squote(arglist, quote, index, fsm);
	else if (fsm->input_state == IN_DQUOTE)
		fsm_env_var_dquote(arglist, quote, index, fsm);
	else if (fsm->input_state == REGULAR)
		fsm_env_var_regular(arglist, quote, index, fsm);
	index->i++;
}

int	has_ended(t_quote *quote, t_index *index, t_arglist *arglist)
{
	if (quote->value[index->i] == '\0')
	{
		if (index->start < index->i && quote->value[index->i - 1] != '\"' && \
			quote->value[index->i - 1] != '$')
		{
			new_argument(arglist, \
			create_expand_arg(&quote->value[index->start], \
				0, index->i - index->start + 1, NON_EXPAND));
		}
		return (1);
	}
	return (0);
}

void	do_fsm_special_case(t_arglist *arglist, \
			t_quote *quote, t_index *index)
{
	if (index->start < index->i)
		new_argument(arglist, \
			create_expand_arg(&quote->value[index->start], 0, \
				index->i - index->start, NON_EXPAND));
	new_argument(arglist, \
		create_expand_arg(&quote->value[index->i + 1], 0, 1, ESCAPED));
	index->i += 1;
	index->start = index->i + 1;
}

void	do_fsm_state_n_input(t_fsm *fsm, t_quote *quote, t_index *index)
{
	if (fsm->input_state == N_INPUT)
	{
		index->start = index->i + 1;
		if (quote->value[index->i] == '\'')
		{
			fsm->input_state = IN_SQUOTE;
			(index->i)++;
		}
		else if (quote->value[index->i] == '"')
		{
			fsm->input_state = IN_DQUOTE;
			(index->i)++;
		}
		else
		{
			fsm->input_state = REGULAR;
			index->start = index->i;
		}
	}
}
