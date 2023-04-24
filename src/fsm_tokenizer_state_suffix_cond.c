/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fsm_tokenizer_state_suffix_cond.c                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 18:00:52 by arobu             #+#    #+#             */
/*   Updated: 2023/04/24 18:04:19 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"
#include <sys/wait.h>
#include "signals.h"

int	is_token_redir_and_none(t_token *token, t_fsm *fsm)
{
	return (is_token_redir(token) && \
		fsm->cmd_p_substate == TOK_CMD_SUFFIX_NONE);
}
