/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var_expansions_chckers.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 21:40:16 by arobu             #+#    #+#             */
/*   Updated: 2023/04/24 21:44:50 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "ft_printf.h"
#include "quote_list.h"
#include "fsm.h"

int	is_special_case(t_quote *quote, t_fsm *fsm, int i)
{
	return (quote->value[i] == '\\' && fsm->input_state != \
			IN_SQUOTE && fsm->input_state != IN_DQUOTE);
}
