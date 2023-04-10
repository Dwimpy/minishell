/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   x_execution.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkilling <tkilling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/05 12:16:27 by tkilling          #+#    #+#             */
/*   Updated: 2023/04/09 13:32:57 by tkilling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# include "ft_printf.h"
#include <stdio.h>

int	ft_command(char **str_arr, t_input *input);

int	ft_cd(char **str_arr, t_input *input);
int	ft_pwd(char **str_arr);
int	ft_echo(char **str_arr, int fd);
int	ft_exit(char **str_arr, t_input *input);
int	ft_env(char **str_arr, t_input *input);
int	ft_export(char **str_arr, t_input *input);
int	ft_unset(char **str_arr, t_input *input);

#endif