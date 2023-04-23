/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fsm_initializer.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/23 20:42:34 by arobu             #+#    #+#             */
/*   Updated: 2023/04/23 20:51:04 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"

void	fsm_init_tokenizer(t_fsm *fsm)
{
	fsm->state = GET_INPUT;
	fsm->input_state = N_INPUT;
	fsm->cmd_state = TOK_CMD_PREFIX;
	fsm->cmd_p_substate = TOK_CMD_PREFIX_NONE;
	fsm->in_subshell = 0;
	fsm->paren = 0;
}

void	fsm_init_tokenizer_params(int *index, int *status, \
		pid_t *pid, char **prompt)
{
	*index = 0;
	*status = 0;
	*pid = 0;
	*prompt = NULL;
}
