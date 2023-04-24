/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fsm_tokenizer_tok_states.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 18:27:12 by arobu             #+#    #+#             */
/*   Updated: 2023/04/24 20:11:58 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"
#include <sys/wait.h>
#include "signals.h"

int	tokenizer_do_tok_cmd(t_token *token, t_fsm *fsm, t_input *input)
{
	if (fsm->tok_state == TOK_CMD)
	{
		if (fsm->cmd_state == TOK_CMD_PREFIX)
		{
			if (tokenizer_tok_cmd_prefix(fsm, token, input))
				return (1);
		}
		else if (fsm->cmd_state == TOK_CMD_NAME)
		{
			if (tokenizer_tok_cmd_name(token, fsm, input))
				return (1);
		}
		else if (fsm->cmd_state == TOK_CMD_SUFFIX)
		{
			if (tokenizer_tok_cmd_suffix(token, fsm, input))
				return (1);
		}
	}
	return (0);
}
