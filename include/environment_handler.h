/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment_handler.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 19:56:59 by arobu             #+#    #+#             */
/*   Updated: 2023/03/31 19:57:54 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENVIRONMENT_HANDLER_H
# define ENVIRONMENT_HANDLER_H

# include "hashmap.h"

t_hashmap	*load_environment(char **envp);
void		load_data(t_hashmap *hashmap, char **envp);

#endif
