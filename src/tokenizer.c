/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/06 19:38:03 by arobu             #+#    #+#             */
/*   Updated: 2023/04/26 13:21:45 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"
#include <sys/wait.h>
#include "signals.h"

int	gen_input(t_input *input)
{
	t_fsm	fsm;
	pid_t	pid;
	char	*prompt;
	int		i;
	int		status;

	fsm_init_tokenizer(&fsm);
	fsm_init_tokenizer_params(&i, &status, &pid, &prompt);
	if (get_tokenizer_input(input))
		return (1);
	add_history(input->lexer.input);
	while (fsm.state != COMPLETE && fsm.state != ERROR)
	{
		get_the_input(input, &fsm);
		tokenize(input, &fsm);
	}
	if (handle_tokenizer_errors(input, &fsm))
		return (1);
	return (0);
}
