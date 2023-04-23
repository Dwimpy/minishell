/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_helpers2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/23 19:02:09 by arobu             #+#    #+#             */
/*   Updated: 2023/04/23 21:31:29 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"
#include <sys/wait.h>
#include "signals.h"

char	*get_prompt_dir(void)
{
	char	path[264];

	if (getcwd(path, sizeof(path)))
	{
		return (ft_strdup(path));
	}
	else
		return (ft_strdup(""));
}

void	do_squote(t_lexer *lexer, t_fsm *fsm)
{
	if (lexer->ch == '\0')
		readline_new_line(lexer, "squote> ");
	else if (lexer->ch == '\'' && fsm->in_subshell)
		fsm->input_state = IN_SUBSH;
	else if (lexer->ch == '\'')
		fsm->input_state = N_INPUT;
}

void	do_dquote(t_lexer *lexer, t_fsm *fsm)
{
	if (lexer->ch == '\0')
		readline_new_line(lexer, "dquote> ");
	else if (lexer->ch == '\"' && fsm->in_subshell)
		fsm->input_state = IN_SUBSH;
	else if (lexer->ch == '\"')
		fsm->input_state = N_INPUT;
}
