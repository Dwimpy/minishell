/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   x_signal_remove_printing.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 17:48:57 by arobu             #+#    #+#             */
/*   Updated: 2023/04/24 17:49:26 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "signals.h"
#include "../readline/readline.h"
#include "../readline/history.h"

void	remove_signal_printing(void)
{
	struct termios		termios_settings;

	tcgetattr(1, &termios_settings);
	termios_settings.c_lflag &= ~ECHOCTL;
	tcsetattr(1, TCSAFLUSH, &termios_settings);
}
