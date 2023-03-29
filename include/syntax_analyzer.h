/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_analyzer.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 12:32:57 by arobu             #+#    #+#             */
/*   Updated: 2023/03/29 13:40:41 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SYNTAX_ANALYZER_H
# define SYNTAX_ANALYZER_H

# include "token_list.h"

typedef enum e_syntax_type
{
	NOTHING,
	IN_CMD,
	IN_PREFIX,
	IN_SUFFIX,
	IN_SUBSHELL,
	IN_PIPE,
	IN_AND_IF,
	IN_OR_IF
}				t_syntax_type;

typedef struct s_fsm
{
	t_syntax_type	state;
}					t_fsm;

int	analyze_syntax(t_token_list *tokens, int *unexpected);
int	accept_prefix(t_token **token, t_fsm *state);
#endif
