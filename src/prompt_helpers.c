/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkilling <tkilling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/23 18:59:19 by arobu             #+#    #+#             */
/*   Updated: 2023/04/24 14:32:45 by tkilling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"
#include <sys/wait.h>
#include "signals.h"

#define CTEAL "\033[0;93m"
#define CTEALBOLD "\033[1;91m"
#define RESET   "\033\e[0m"
#define PROMPT "\033[0;91;1m > "

void	readline_new_line(t_lexer *lexer, char *prompt)
{
	char	*append_line;
	char	*join_line;

	append_line = readline(prompt);
	if (!append_line)
		return ;
	join_line = (char *)malloc(sizeof(char) * (ft_strlen(lexer->input) + \
				ft_strlen(append_line) + 2));
	ft_strcpy(join_line, lexer->input);
	ft_strcat(join_line, "\n");
	ft_strcat(join_line, append_line);
	free(lexer->input);
	free(append_line);
	lexer->input = join_line;
	lexer->input_len = ft_strlen(lexer->input);
	lexer->read_position--;
}

int	readline_no_new_line(t_lexer *lexer, char *prompt)
{
	char	*append_line;
	char	*join_line;

	append_line = readline(prompt);
	if (!append_line)
		return (1);
	join_line = (char *)malloc(sizeof(char) * (ft_strlen(lexer->input) + \
				ft_strlen(append_line) + 2));
	ft_strcpy(join_line, lexer->input);
	ft_strcat(join_line, append_line);
	free(lexer->input);
	free(append_line);
	lexer->input = join_line;
	lexer->input_len = ft_strlen(lexer->input);
	lexer->read_position--;
	return (0);
}

int	readline_pipe(t_lexer *lexer, char *prompt)
{
	char	*append_line;
	char	*join_line;

	append_line = readline(prompt);
	if (!append_line)
		return 1;
	while (append_line && is_empty(append_line))
	{
		free(append_line);
		append_line = readline(prompt);
	}
	if (!append_line)
		return (1);
	join_line = (char *)malloc(sizeof(char) * (ft_strlen(lexer->input) + \
				ft_strlen(append_line) + 2));
	ft_strcpy(join_line, lexer->input);
	ft_strcat(join_line, append_line);
	free(lexer->input);
	free(append_line);
	lexer->input = join_line;
	lexer->input_len = ft_strlen(lexer->input);
	lexer->read_position--;
	lexer->tok_position--;
	return (0);
}

void	do_linebreak(t_lexer *lexer, char *prompt, t_fsm *fsm)
{
	char	*append_line;
	char	*join_line;
	char	*trimmed;

	append_line = readline(prompt);
	if (!append_line)
		return ;
	if (append_line[0] == '\0')
	{
		fsm->input_state = INPUT_COMPLETE;
		free(append_line);
		return ;
	}
	trimmed = ft_strtrim(lexer->input, "\\");
	join_line = (char *)malloc(sizeof(char) * (ft_strlen(lexer->input) + \
				ft_strlen(append_line) + 1));
	ft_strcpy(join_line, trimmed);
	ft_strcat(join_line, append_line);
	free(lexer->input);
	free(trimmed);
	free(append_line);
	lexer->input = join_line;
	lexer->input_len = ft_strlen(lexer->input);
	lexer->read_position -= 2;
	fsm->input_state = N_INPUT;
}

char    *read_from_stdin(t_input *input)
{
	char	*prompt;
	char	*line;
	char	*user;
	char	*new;
	size_t	len_user;

	ft_signals(&(input->sa), 1);
	prompt = get_prompt_dir();
	user =  (char *)hashmap_get(input->special_sym, "TILDE");
	if (user && !ft_strncmp(prompt, user, ft_strlen(user)))
	{
		len_user = ft_strlen(user);
		if (len_user == ft_strlen(prompt))
		{
			new = ft_calloc(ft_strlen(CTEALBOLD) + ft_strlen(RESET) + ft_strlen(PROMPT) + 4, sizeof(char));
			ft_strcat(new, CTEALBOLD);
			ft_strcat(new, "~ ");
			ft_strcat(new, PROMPT);
			ft_strcat(new, RESET);
		}
		else
		{
			new = ft_calloc(len_user + ft_strlen(&prompt[len_user]) + ft_strlen(CTEALBOLD) + ft_strlen(CTEAL) + ft_strlen(RESET) + ft_strlen(PROMPT) + 1, sizeof(char));
			ft_strcat(new, CTEAL);
			ft_strcat(new, "~");
			ft_strncat(new, &prompt[len_user], ft_strrchr(prompt, '/') - &prompt[len_user] + 1);
			ft_strcat(new, CTEALBOLD);
			ft_strcat(new, ft_strrchr(prompt, '/') + 1);
			ft_strcat(new, PROMPT);
			ft_strcat(new, RESET);
		}
		line = readline(new);
		free(new);
    }
    else
    {
        new = ft_calloc(ft_strlen(CTEAL) +ft_strlen(CTEAL) + ft_strlen(RESET) + ft_strlen(PROMPT) + ft_strlen(prompt) + 2, sizeof(char));
        ft_strcat(new, CTEAL);
        ft_strcat(new, "/");
        ft_strcat(new, CTEALBOLD);
        if (prompt && &prompt[1])
            ft_strcat(new, &prompt[1]);
        ft_strcat(new, PROMPT);
        ft_strcat(new, RESET);
        line = readline(new);
        free(new);
    }
    free(prompt);
    ft_signals(&(input->sa), 0);
    return (line);
}
