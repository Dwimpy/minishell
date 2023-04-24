/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkilling <tkilling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/22 21:22:49 by arobu             #+#    #+#             */
/*   Updated: 2023/04/24 11:12:52 by tkilling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <signal.h>
# include <termios.h>

int		ft_signals(struct sigaction *sa, int is_reading);
int		ft_signals_child(struct sigaction *sa);
void	ft_signal_handler_reading(int sig, siginfo_t *info, void *context);
void	ft_signal_handler_executing(int sig, siginfo_t *info, void *context);
void	ft_signal_handler_child(int sig, siginfo_t *info, void *context);
void	remove_signal_printing(void);

#endif
