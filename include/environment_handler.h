/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment_handler.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 19:56:59 by arobu             #+#    #+#             */
/*   Updated: 2023/04/24 21:42:36 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENVIRONMENT_HANDLER_H
# define ENVIRONMENT_HANDLER_H

# include "hashmap.h"
# include "ft_printf.h"

t_hashmap	*load_environment(char **envp);
void		load_data(t_hashmap *hashmap, char **envp);

#endif
