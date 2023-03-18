/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_list.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 12:23:45 by arobu             #+#    #+#             */
/*   Updated: 2023/03/18 14:18:59 by arobu            ###   ########.fr       */
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
void			free_token_list(t_token_list *token_list);

#endif
