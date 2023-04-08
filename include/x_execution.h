/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   x_execution.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkilling <tkilling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/05 12:16:27 by tkilling          #+#    #+#             */
/*   Updated: 2023/04/08 11:37:36 by tkilling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# include "ft_printf.h"
#include <stdio.h>

int	ft_command(char **str_arr, t_input *input);

void	ft_cd(char **str_arr, t_input *input);
void	ft_pwd(char **str_arr);
void	ft_echo(char **str_arr, int fd);
void	ft_exit(char **str_arr, t_input *input);
void	ft_env(char **str_arr, t_input *input);
void	ft_export(char **str_arr, t_input *input);
void	ft_unset(char **str_arr, t_input *input);

#endif