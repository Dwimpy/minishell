/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   x_execution.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/05 12:16:27 by tkilling          #+#    #+#             */
/*   Updated: 2023/04/16 19:01:23 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef X_EXECUTION_H
# define X_EXECUTION_H

# include "ft_printf.h"
#include <stdio.h>

int			ft_command(char **str_arr, t_input *input, t_ast_node *root);
int			ft_cd(char **str_arr, t_input *input);
int			ft_pwd(char **str_arr);
int			ft_echo(char **str_arr, int fd);
int			ft_exit(char **str_arr, t_input *input);
int			ft_env(char **str_arr, t_input *input);
int			ft_export(char **str_arr, t_input *input);
int			ft_unset(char **str_arr, t_input *input);
int			ft_executable(char **str_arr, t_input *input);
int			ft_executable_no_env(char **str_arr, t_input *input);
char		*get_env_vars(t_arglist *list, t_input *input);
t_arglist	*expand_vars(char	*value);
void		expand_env_vars(char **args, t_input *input);

#endif