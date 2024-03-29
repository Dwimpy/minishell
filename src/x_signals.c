/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   x_signals.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/22 20:48:55 by tkilling          #+#    #+#             */
/*   Updated: 2023/04/26 13:23:37 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "parser.h"
// #include "tokenizer.h"
// #include "environment_handler.h"
// #include "get_next_line.h"
#include "signals.h"
#include "../readline/readline.h"
#include "../readline/history.h"

void	remove_signal_printing(void);

int	ft_signals(struct sigaction *sa, int is_reading)
{
	remove_signal_printing();
	sa->sa_flags = SA_SIGINFO;
	if (is_reading)
		sa->sa_sigaction = ft_signal_handler_reading;
	else
		sa->sa_sigaction = ft_signal_handler_executing;
	signal(SIGQUIT, SIG_IGN);
	if (!is_reading && sigaction(SIGQUIT, sa, NULL) != 0)
	{
		write(2, "sigaction error\n", 16);
		exit (1);
	}
	if (sigaction(SIGINT, sa, NULL) != 0)
	{
		write(2, "sigaction error\n", 16);
		exit (1);
	}
	return (0);
}

int	ft_signals_child(struct sigaction *sa)
{
	sa->sa_flags = SA_SIGINFO;
	sa->sa_sigaction = ft_signal_handler_child;
	if (sigaction(SIGQUIT, sa, NULL) != 0 || sigaction(SIGINT, sa, NULL) != 0)
	{
		write(2, "sigaction error\n", 16);
		exit (1);
	}
	return (0);
}

void	ft_signal_handler_reading(int sig, siginfo_t *info, void *context)
{
	(void)info;
	(void)context;
	if (sig == 2)
	{
		rl_replace_line("", 0);
		write(1, "\n", 2);
		rl_on_new_line();
		rl_redisplay();
	}
}

void	ft_signal_handler_executing(int sig, siginfo_t *info, void *context)
{
	(void)info;
	(void)context;
	if (sig == 2)
	{
		write(1, "^C\n", 4);
	}
	if (sig == 3)
	{
		write(1, "^\\Quit: 3\n", 10);
	}
}

void	ft_signal_handler_child(int sig, siginfo_t *info, void *context)
{
	(void)info;
	(void)context;
	if (sig == 2)
	{
		exit (0);
	}
	if (sig == 3)
	{
		exit (0);
	}
}
