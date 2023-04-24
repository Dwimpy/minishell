/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_matching.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 09:57:09 by arobu             #+#    #+#             */
/*   Updated: 2023/04/24 22:12:55 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	is_prefix(t_token *token)
{
	return (is_input_redir(token) || \
				is_output_redir(token) || \
					token->type == TOKEN_ASSIGN_WORD);
}

int	is_cmd_suffix(t_token *token)
{
	return (is_cmd_word(token) || \
			is_input_redir(token) || \
				is_output_redir(token) || \
					is_assign_word(token));
}

int	is_input_redir(t_token *token)
{
	return (token->type == TOKEN_LESS || token->type == TOKEN_DLESS);
}

int	is_output_redir(t_token *token)
{
	return (token->type == TOKEN_GREAT || token->type == TOKEN_DGREAT);
}

int	is_assign_word(t_token *token)
{
	return (token->type == TOKEN_ASSIGN_WORD);
}
