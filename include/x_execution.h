/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   x_execution.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/05 12:16:27 by tkilling          #+#    #+#             */
/*   Updated: 2023/04/15 00:05:50 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef X_EXECUTION_H
# define X_EXECUTION_H

# include "ft_printf.h"
#include <stdio.h>

int		ft_command(char **str_arr, t_input *input, t_ast_node *root);
int		ft_cd(char **str_arr, t_input *input);
int		ft_pwd(char **str_arr);
int		ft_echo(char **str_arr, int fd);
int		ft_exit(char **str_arr, t_input *input);
int		ft_env(char **str_arr, t_input *input);
int		ft_export(char **str_arr, t_input *input);
int		ft_unset(char **str_arr, t_input *input);
void	expand_node_cmds(char **arr, t_input *input);
#endif
