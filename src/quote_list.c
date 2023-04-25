/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 19:36:15 by arobu             #+#    #+#             */
/*   Updated: 2023/04/25 17:01:42 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quote_list.h"
#include "fsm.h"
#include "ft_printf.h"
#include <stdio.h>

static void	do_quotelist_regular(char *value, t_quotelist *list, \
			t_index *index, t_fsm *fsm);
static void	do_quotelist_squote(char *value, t_quotelist *list, \
			t_index *index, t_fsm *fsm);
static void	do_quotelist_dquote(char *value, t_quotelist *list, \
			t_index *index, t_fsm *fsm);
static void	do_quotelist_ninput(char *value, \
			t_index *index, t_fsm *fsm);

t_quotelist	*create_list(char *value)
{
	t_index		index;
	t_fsm		fsm;
	t_quotelist	*list;

	index.i = 0;
	if (!value)
		return (NULL);
	list = new_quotelist();
	fsm.input_state = N_INPUT;
	while (fsm.input_state != INPUT_COMPLETE)
	{
		if (end_of_input(value, &index, &fsm))
			break ;
		if (fsm.input_state == N_INPUT)
			do_quotelist_ninput(value, &index, &fsm);
		else if (fsm.input_state == IN_SQUOTE)
			do_quotelist_squote(value, list, &index, &fsm);
		else if (fsm.input_state == IN_DQUOTE)
			do_quotelist_dquote(value, list, &index, &fsm);
		else if (fsm.input_state == REGULAR)
			do_quotelist_regular(value, list, &index, &fsm);
		index.i++;
	}
	return (list);
}

static void	do_quotelist_ninput(char *value, \
			t_index *index, t_fsm *fsm)
{
	if (value[index->i] == '\'')
		fsm->input_state = IN_SQUOTE;
	else if (value[index->i] == '\"')
		fsm->input_state = IN_DQUOTE;
	else
	{
		fsm->input_state = REGULAR;
	}
	index->start = index->i;
	if (value[index->i + 1] == '\0')
		index->i -= 1;
}

static void	do_quotelist_squote(char *value, t_quotelist *list, \
			t_index *index, t_fsm *fsm)
{
	if (value[index->i] == '\'')
	{
		fsm->input_state = N_INPUT;
		add_quote_element(list, new_quote_elem(ft_substr(value, \
			index->start, index->i - index->start + 1), QUOTE_SQUOTE));
	}
}

static void	do_quotelist_dquote(char *value, t_quotelist *list, \
			t_index *index, t_fsm *fsm)
{
	if (value[index->i] == '\"')
	{
		fsm->input_state = N_INPUT;
		add_quote_element(list, new_quote_elem(ft_substr(value, \
			index->start, index->i - index->start + 1), QUOTE_DQUOTE));
	}
}

static void	do_quotelist_regular(char *value, t_quotelist *list, \
			t_index *index, t_fsm *fsm)
{
	if (value[index->i] == '\'')
	{
		fsm->input_state = IN_SQUOTE;
		add_quote_element(list, new_quote_elem(ft_substr(value, \
			index->start, index->i - index->start), QUOTE_REGULAR));
		index->start = index->i;
	}
	else if (value[index->i] == '\"')
	{
		fsm->input_state = IN_DQUOTE;
		add_quote_element(list, new_quote_elem(ft_substr(value, \
			index->start, index->i - index->start), QUOTE_REGULAR));
		index->start = index->i;
	}
	else if (value[index->i + 1] == '\0')
	{
		add_quote_element(list, new_quote_elem(ft_substr(value, \
			index->start, index->i - index->start + 1), QUOTE_REGULAR));
	}
}
