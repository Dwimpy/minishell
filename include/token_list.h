/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_list.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 12:23:45 by arobu             #+#    #+#             */
/*   Updated: 2023/03/21 12:25:44 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_LIST_H
# define TOKEN_LIST_H
# include "token.h"
# include <stdlib.h>

typedef struct s_token_list
{
	t_token		*first;
	t_token		*last;
	size_t		num_tokens;
}				t_token_list;

t_token_list	*new_token_list(void);
void			add_token(t_token_list *token_list, t_token *token);
void			remove_token(t_token_list *token_list);
void			print_tokens(t_token_list *list);
t_token			*get_next_token(t_token *token);
t_token			*peek_token(t_token_list *list, int look_ahead_times);
void			consume_token(t_token_list *tokens);
int				get_nargs(t_token *token);
void			free_token_list(t_token_list *token_list);

#endif
