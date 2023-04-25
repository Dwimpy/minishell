/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_free.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 17:30:12 by arobu             #+#    #+#             */
/*   Updated: 2023/04/25 17:31:45 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	free_loop(t_input *input, int *exit_code)
{
	if (*exit_code >= 256)
		*exit_code = *exit_code / 256;
	free(hashmap_put(input->special_sym, "EXITSTATUS", ft_itoa(*exit_code)));
	ast_del_node(input->root);
	input->root = NULL;
	free(input->lexer.input);
	free_token_list(input->tokens);
	free_heredoc_list(input->heredoc_files);
}
