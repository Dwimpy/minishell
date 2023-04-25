/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 01:43:09 by arobu             #+#    #+#             */
/*   Updated: 2023/04/25 02:54:07 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"
#include "parser.h"
#include "x_execution.h"

void	parse_input(t_ast_node **root, t_token_list *tokens, \
		t_input *input, size_t	*sub_count)
{
	parse_subshell(tokens, sub_count);
	ast_add(root, parse_command(tokens, input, *sub_count));
	parse_subshell(tokens, sub_count);
	ast_add(root, parse_pipeline(tokens, *sub_count));
	parse_subshell(tokens, sub_count);
	ast_add(root, parse_and_if(tokens, *sub_count));
	parse_subshell(tokens, sub_count);
	ast_add(root, parse_or_if(tokens, *sub_count));
	parse_subshell(tokens, sub_count);
	ast_add(root, parse_subshell(tokens, sub_count));
}
