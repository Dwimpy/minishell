/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   x_remove_signal_printing.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkilling <tkilling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 11:11:08 by tkilling          #+#    #+#             */
/*   Updated: 2023/04/24 11:11:49 by tkilling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "signals.h"

void	remove_signal_printing(void)
{
	struct termios		termios_settings;

	tcgetattr(1, &termios_settings);
	termios_settings.c_lflag &= ~ECHOCTL;
	tcsetattr(1, TCSAFLUSH, &termios_settings);
}
