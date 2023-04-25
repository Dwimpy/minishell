/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_from_stdin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 00:50:12 by arobu             #+#    #+#             */
/*   Updated: 2023/04/25 01:03:59 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"
#include <sys/wait.h>
#include "signals.h"

#define CTEAL "\033[0;93m"
#define CTEALBOLD "\033[1;91m"
#define RESET   "\033\e[0m"
#define PROMPT "\033[0;91;1m > "

static char	*get_new_prompt(void);
static char	*get_new_prompt_2(char *prompt, size_t len_user);
static char	*get_new_prompt_3(char *prompt);
static char	*read_stdin(char *user, char *prompt);

char	*read_from_stdin(t_input *input)
{
	char	*prompt;
	char	*line;
	char	*user;

	ft_signals(&(input->sa), 1);
	prompt = get_prompt_dir();
	user = (char *)hashmap_get(input->special_sym, "TILDE");
	line = read_stdin(user, prompt);
	free(prompt);
	ft_signals(&(input->sa), 0);
	return (line);
}

static char	*read_stdin(char *user, char *prompt)
{
	char	*new;
	char	*line;
	size_t	len_user;

	new = NULL;
	line = NULL;
	len_user = 0;
	if (user && !ft_strncmp(prompt, user, ft_strlen(user)))
	{
		len_user = ft_strlen(user);
		if (len_user == ft_strlen(prompt))
			new = get_new_prompt();
		else
			new = get_new_prompt_2(prompt, len_user);
		line = readline(new);
		free(new);
	}
	else
	{
		new = get_new_prompt_3(prompt);
		line = readline(new);
		free(new);
	}
	return (line);
}

static char	*get_new_prompt(void)
{
	char	*new;

	new = ft_calloc(ft_strlen(CTEALBOLD) + \
			ft_strlen(RESET) + \
				ft_strlen(PROMPT) + 4, sizeof(char));
	ft_strcat(new, CTEALBOLD);
	ft_strcat(new, "~ ");
	ft_strcat(new, PROMPT);
	ft_strcat(new, RESET);
	return (new);
}

static char	*get_new_prompt_2(char *prompt, size_t len_user)
{
	char	*new;

	new = ft_calloc(len_user + ft_strlen(&prompt[len_user]) + \
			ft_strlen(CTEALBOLD) + ft_strlen(CTEAL) + ft_strlen(RESET) + \
			ft_strlen(PROMPT) + 1, sizeof(char));
	ft_strcat(new, CTEAL);
	ft_strcat(new, "~");
	ft_strncat(new, &prompt[len_user], \
		ft_strrchr(prompt, '/') - &prompt[len_user] + 1);
	ft_strcat(new, CTEALBOLD);
	ft_strcat(new, ft_strrchr(prompt, '/') + 1);
	ft_strcat(new, PROMPT);
	ft_strcat(new, RESET);
	return (new);
}

static char	*get_new_prompt_3(char *prompt)
{
	char	*new;

	new = ft_calloc(ft_strlen(CTEAL) +ft_strlen(CTEAL) + \
		ft_strlen(RESET) + ft_strlen(PROMPT) + \
			ft_strlen(prompt) + 2, sizeof(char));
	ft_strcat(new, CTEAL);
	ft_strcat(new, "/");
	ft_strcat(new, CTEALBOLD);
	if (prompt && &prompt[1])
		ft_strcat(new, &prompt[1]);
	ft_strcat(new, PROMPT);
	ft_strcat(new, RESET);
	return (new);
}
