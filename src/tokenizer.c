/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/06 19:38:03 by arobu             #+#    #+#             */
/*   Updated: 2023/04/23 18:50:11 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"
#include <sys/wait.h>
#include "signals.h"

#define CTEAL "\033[0;93m"
#define CTEALBOLD "\033[1;91m"
#define RESET   "\033\e[0m"
#define PROMPT "\033[0;91;1m > "

int	gen_input(t_input *input)
{
	t_fsm	fsm;
	pid_t	pid;
	char	*prompt;
	int		i;
	int		status;

	fsm.state = GET_INPUT;
	fsm.input_state = N_INPUT;
	fsm.cmd_state = TOK_CMD_PREFIX;
	fsm.cmd_p_substate = TOK_CMD_PREFIX_NONE;
	fsm.in_subshell = 0;
	fsm.paren = 0;
	i = 0;
	status = 0;
	pid = 0;
	prompt = NULL;
	input->lexer.input = read_from_stdin(input);
	if (!input->lexer.input)
	{
		exit(ft_atoi((char *)hashmap_get(input->special_sym, "EXITSTATUS")));
	}
	input->lexer.input_len = ft_strlen(input->lexer.input);
	while (input->lexer.input && (input->lexer.input[i] == ' ' || \
		input->lexer.input[i] == '\t'))
		i++;
	if (i == input->lexer.input_len)
	{
		free(input->lexer.input);
		return (1);
	}
	input->lexer.read_position = -1;
	input->lexer.tok_position = -1;
	input->unexpected = 0;
	while (fsm.state != COMPLETE && fsm.state != ERROR)
	{
		get_the_input(input, &fsm);
		tokenize(input, &fsm);
	}
	if (fsm.state == ERROR)
	{
			print_syntax_error(input->unexpected);
			free(input->lexer.input);
			free_token_list(input->tokens);
			free(hashmap_put(input->special_sym, "EXITSTATUS", ft_strdup("2")));
			free_heredoc_list(input->heredoc_files);
			return (1);
	}
	// if (!ft_strncmp(input->tokens->first->value.word.value, "../", 4) &&
	// 	ft_strlen(input->tokens->first->value.word.value) == 3)
	// {
	// 	input->unexpected = '.';
	// 	ft_putstr_fd("bash: .: filename argument required\n", 2);
	// 	ft_putstr_fd(input->tokens->first->value.word.value, 2);
	// 	ft_putstr_fd(": usage: . filename [arguments]\n", 2);
	// 	return (1);
	// }
	// else if (!ft_strncmp(input->tokens->first->value.word.value, ".", 2) &&
	// 	ft_strlen(input->tokens->first->value.word.value) == 1)
	// {
	// 	input->unexpected = 21;
	// 	ft_putstr_fd("bash: .: filename argument required\n", 3);
	// 	ft_putstr_fd(input->tokens->first->value.word.value, 2);
	// 	ft_putstr_fd(": usage: . filename [arguments]\n", 2);
	// 	return (1);
	// }
	if (fsm.state == COMPLETE)
		add_token(input->tokens, new_token(TOKEN_EOF, NULL, EMPTY));
	// print_tokens(input->tokens);
	add_history(input->lexer.input);
	return (0);
}

void	tokenize(t_input *input, t_fsm *fsm)
{
	t_lexer			*lexer;
	t_token			*token;
	t_token_list	*tokens;

	lexer = &input->lexer;
	if (fsm->state == ERROR)
		return ;
	fsm->state = GET_TOKENS;
	fsm->tok_state = N_TOKENIZER;
	token = NULL;
	tokens = input->tokens;
	while (fsm->tok_state != TOK_COMPLETE && \
			fsm->state != INCOMPLETE && \
			fsm->state != COMPLETE && \
			fsm->state != ERROR)
	{
		token = create_next_token(lexer);

		// print_token_value(token);
		if (!token)
		{
			if (!tokens->last)
				fsm->state = COMPLETE;
			else if (is_tokenizer_ending(input))
			{
				fsm->state = INCOMPLETE;
				if (tokens->last->type == TOKEN_PIPE)
					fsm->tok_state = TOK_PIPE;
				else if (tokens->last->type == TOKEN_AND_IF)
					fsm->tok_state = TOK_AND_IF;
				else if (tokens->last->type == TOKEN_OR_IF)
					fsm->tok_state = TOK_OR_IF;
				else if (is_token_redir(tokens->last))
				{
					fsm->state = ERROR;
					input->unexpected = TOKEN_EOF;
				}
			}
			else
			{
				fsm->tok_state = TOK_COMPLETE;
				fsm->state = COMPLETE;
			}
			return ;
		}
		else
		{
			if (input->tokens->num_tokens == 0)
			{
				if (!is_valid_beginning(token))
				{
					// printf("ERROR IN STATE %d\n", fsm->tok_state);
					fsm->state = ERROR;
					input->unexpected = token->type;
					break ;
				}
			}
			if (fsm->tok_state == N_TOKENIZER)
			{
				if (is_prefix(token) || is_cmd_suffix(token))
				{
					fsm->tok_state = TOK_CMD;
					fsm->cmd_state = TOK_CMD_PREFIX;
					fsm->cmd_p_substate = TOK_CMD_PREFIX_NONE;
					fsm->redir_state = OTHER;
				}
				else if (is_token_lparen(token))
					fsm->tok_state = TOK_LPARENTHESIS;
				else if (is_token_rparen(token))
					fsm->tok_state = TOK_RPARENTHESIS;
				else if (is_token_pipe(token))
					fsm->tok_state = TOK_PIPE;
				else if (is_token_cmdand(token))
					fsm->tok_state = TOK_AND_IF;
				else if (is_token_cmdor(token))
					fsm->tok_state = TOK_OR_IF;
			}
		}
		if (fsm->tok_state == TOK_CMD)
		{
			if (fsm->cmd_state == TOK_CMD_PREFIX)
			{
				if (is_token_redir(token) && \
					fsm->cmd_p_substate == TOK_CMD_PREFIX_NONE)
				{
					fsm->cmd_p_substate = TOK_CMD_PREFIX_REDIR;
					if (token->type == TOKEN_DLESS)
					{
						fsm->redir_state = HEREDOC;
					}
					else
						fsm->redir_state = OTHER;
				}
				else if (fsm->cmd_p_substate == TOK_CMD_PREFIX_REDIR)
				{
					if (fsm->redir_state == OTHER)
					{
						if (!is_token_word_literal(token))
						{
							fsm->state = ERROR;
							input->unexpected = token->type;
						}
					}
					else if (fsm->redir_state == HEREDOC)
					{
						char	*line;
						char	*filename;
						int		fd;
						char	*expanded;

						line = NULL;
						if (!is_token_word_literal(token))
						{
							fsm->state = ERROR;
							input->unexpected = token->type;
						}
						if (token->type == TOKEN_WORD)
							filename = get_env_vars(expand_vars(token->value.word.value), input);
						else if (token->type == TOKEN_QUOTE)
							filename = get_env_vars(expand_vars(token->value.quote.value), input);
						fd = open(filename, O_CREAT | O_WRONLY | O_APPEND, 0644);
						if (fd > 0)
						{
							new_argument(input->heredoc_files, create_heredoc_file(filename));
							while(1)
							{
								char	*str;

								line = readline("heredoc> ");
								if (!line)
									break ;
								if (token->type == TOKEN_WORD)
								{
									// printf("%s\n", str);
									if (!ft_strncmp(line, token->value.word.value, ft_strlen(token->value.word.value)))
									{
										free(line);
										break ;
									}
									expanded = get_env_vars(expand_vars(line), input);
									ft_putstr_fd(expanded, fd);
									ft_putstr_fd("\n", fd);
									free(line);
									free(expanded);
								}
								else if (token->type == TOKEN_QUOTE)
								{
									str = get_env_vars(expand_vars(token->value.quote.value), input);
									if (!ft_strncmp(line, str, ft_strlen(str)))
									{
										free(str);
										free(line);
										break ;
									}
									expanded = get_env_vars(expand_vars(line), input);
									ft_putstr_fd(expanded, fd);
									ft_putstr_fd("\n", fd);
									free(line);
									free(expanded);
									free(str);
								}
							}
						}
					}
					fsm->cmd_p_substate = TOK_CMD_PREFIX_NONE;
					fsm->redir_state = OTHER;
				}
				else if (is_token_assignment(token) && \
					fsm->cmd_p_substate == TOK_CMD_PREFIX_NONE)
					;
				else if (is_token_word_literal(token) && \
					fsm->cmd_p_substate == TOK_CMD_PREFIX_NONE)
					fsm->cmd_state = TOK_CMD_NAME;
				else if (is_token_rparen(token) && \
					fsm->cmd_p_substate == TOK_CMD_PREFIX_NONE)
				{
					add_token(tokens, token);
					fsm->tok_state = TOK_RPARENTHESIS;
					continue ;
				}
				else if (is_token_logical_op(token) && \
					fsm->cmd_state == TOK_CMD_PREFIX_NONE)
				{
					if (token->type == TOKEN_PIPE)
						fsm->tok_state = TOK_PIPE;
					else if (token->type == TOKEN_AND_IF)
						fsm->tok_state = TOK_AND_IF;
					else if (token->type == TOKEN_OR_IF)
						fsm->tok_state = TOK_OR_IF;
				}
				else
				{
					// printf("ERROR IN STATE %d\n", fsm->tok_state);
					// printf("ERROR IN P_STATE %d\n", fsm->cmd_p_substate);
					fsm->state = ERROR;
					input->unexpected = token->type;
				}
			}
			else if (fsm->cmd_state == TOK_CMD_NAME)
			{
				if (is_token_word_literal(token) || is_token_assignment(token))
					;
				else if (is_token_redir(token))
				{
					fsm->cmd_state = TOK_CMD_SUFFIX;
					fsm->cmd_p_substate = TOK_CMD_SUFFIX_REDIR;
					if (token->type == TOKEN_DLESS)
					{
						fsm->redir_state = HEREDOC;
					}
					else
						fsm->redir_state = OTHER;
				}
				else if (is_token_logical_op(token))
				{
					add_token(tokens, token);
					if (token->type == TOKEN_PIPE)
						fsm->tok_state = TOK_PIPE;
					else if (token->type == TOKEN_AND_IF)
						fsm->tok_state = TOK_AND_IF;
					else if (token->type == TOKEN_OR_IF)
						fsm->tok_state = TOK_OR_IF;
					continue ;
				}
				else if (is_token_rparen(token))
					fsm->tok_state = TOK_RPARENTHESIS;
				else
				{
					// printf("ERROR IN STATE %d\n", fsm->tok_state);
					// printf("ERROR IN P_STATE %d\n", fsm->cmd_p_substate);

					fsm->state = ERROR;
					input->unexpected = token->type;
				}
			}
			else if (fsm->cmd_state == TOK_CMD_SUFFIX)
			{
				if (is_token_redir(token) && \
					fsm->cmd_p_substate == TOK_CMD_SUFFIX_NONE)
				{
					fsm->cmd_p_substate = TOK_CMD_SUFFIX_REDIR;
					if (token->type == TOKEN_DLESS)
					{
						fsm->redir_state = HEREDOC;
					}
					else
						fsm->redir_state = OTHER;
				}
				else if (fsm->cmd_p_substate == TOK_CMD_SUFFIX_REDIR)
				{
					if (fsm->redir_state == OTHER)
					{
						if (!is_token_word_literal(token))
						{
							fsm->state = ERROR;
							input->unexpected = token->type;
						}
					}
					else if (fsm->redir_state == HEREDOC)
					{
						char	*line;
						char	*filename;
						int		fd;
						char	*expanded;

						line = NULL;
						if (!is_token_word_literal(token))
						{
							fsm->state = ERROR;
							input->unexpected = token->type;
						}
						if (token->type == TOKEN_WORD)
							filename = get_env_vars(expand_vars(token->value.word.value), input);
						else if (token->type == TOKEN_QUOTE)
							filename = get_env_vars(expand_vars(token->value.quote.value), input);
						fd = open(filename, O_CREAT | O_WRONLY | O_APPEND, 0644);
						if (fd > 0)
						{
							new_argument(input->heredoc_files, create_heredoc_file(filename));
							while(1)
							{
								char	*str;

								line = readline("heredoc> ");
								if (!line)
									break ;
								if (token->type == TOKEN_WORD)
								{
									// printf("%s\n", str);
									if (!ft_strncmp(line, token->value.word.value, ft_strlen(token->value.word.value)))
									{
										free(line);
										break ;
									}
									expanded = get_env_vars(expand_vars(line), input);
									ft_putstr_fd(expanded, fd);
									ft_putstr_fd("\n", fd);
									free(line);
									free(expanded);
								}
								else if (token->type == TOKEN_QUOTE)
								{
									str = get_env_vars(expand_vars(token->value.quote.value), input);
									if (!ft_strncmp(line, str, ft_strlen(str)))
									{
										free(str);
										free(line);
										break ;
									}
									expanded = get_env_vars(expand_vars(line), input);
									ft_putstr_fd(expanded, fd);
									ft_putstr_fd("\n", fd);
									free(line);
									free(str);
									free(expanded);
								}
							}
						}
					}
					fsm->cmd_p_substate = TOK_CMD_SUFFIX_NONE;
					fsm->redir_state = OTHER;
				}
				else if (is_token_word_literal(token) || is_token_assignment(token))
					;
				else if (is_token_logical_op(token))
				{
					if (token->type == TOKEN_PIPE)
						fsm->tok_state = TOK_PIPE;
					else if (token->type == TOKEN_AND_IF)
						fsm->tok_state = TOK_AND_IF;
					else if (token->type == TOKEN_OR_IF)
						fsm->tok_state = TOK_OR_IF;
				}
				else if (is_token_rparen(token))
				{
					add_token(tokens, token);
					fsm->tok_state = TOK_RPARENTHESIS;
					continue ;
				}
				else
				{
					// printf("ERROR IN STATE %d\n", fsm->tok_state);
					// printf("ERROR IN P_STATE %d\n", fsm->cmd_p_substate);

					fsm->state = ERROR;
					input->unexpected = token->type;
				}
			}
		}
		else if (is_tok_state_pipe_lop(fsm))
		{
			if (is_prefix(token) || is_cmd_suffix(token))
			{
				fsm->tok_state = TOK_CMD;
				if (is_token_redir(token))
				{
				fsm->cmd_state = TOK_CMD_PREFIX;
				fsm->cmd_p_substate = TOK_CMD_PREFIX_REDIR;
				}
				else
				{
					fsm->cmd_state = TOK_CMD_PREFIX;
					fsm->cmd_p_substate = TOK_CMD_PREFIX_NONE;
				}
			}
			else if (is_token_lparen(token))
				fsm->tok_state = TOK_LPARENTHESIS;
			else
			{
					// printf("ERROR IN STATE %d\n", fsm->tok_state);

				fsm->state = ERROR;
				input->unexpected = token->type;
			}
		}
		else if (fsm->tok_state == TOK_LPARENTHESIS)
		{
			if (token->type == TOKEN_LPARENTHESIS)
				;
			else if (is_token_redir(token))
			{
				fsm->tok_state = TOK_CMD;
				fsm->cmd_state = TOK_CMD_PREFIX;
				fsm->cmd_p_substate = TOK_CMD_PREFIX_REDIR;
			}
			else if (is_token_word_literal(token))
			{
				fsm->tok_state = TOK_CMD;
				fsm->cmd_state = TOK_CMD_PREFIX;
				fsm->cmd_p_substate = TOK_CMD_PREFIX_NONE;
			}
			else 
			{
				fsm->state = ERROR;
				input->unexpected = token->type;
			}
		}
		else if (fsm->tok_state == TOK_RPARENTHESIS)
		{
			if (is_token_logical_op(token))
			{
				if (token->type == TOKEN_PIPE)
					fsm->tok_state = TOK_PIPE;
				else if (token->type == TOKEN_AND_IF)
					fsm->tok_state = TOK_AND_IF;
				else if (token->type == TOKEN_OR_IF)
					fsm->tok_state = TOK_OR_IF;
			}
			else if (is_token_rparen(token))
			{
				add_token(tokens, token);
				continue ;
			}
			else
			{
				fsm->state = ERROR;
				input->unexpected = token->type;
			}
		}
		add_token(tokens, token);
	}
}

void	get_the_input(t_input *input, t_fsm *fsm)
{
	t_lexer		*lexer;
	char		c;

	lexer = &input->lexer;
	fsm->input_state = N_INPUT;
	while (fsm->input_state != INPUT_COMPLETE)
	{
		if (fsm->state == GET_INPUT)
			run_fsm_input_get_input(lexer, fsm, input);
		else if (fsm->state == INCOMPLETE)
			do_fsm_incomplete_state(lexer, fsm, input);
	}
}

void	run_fsm_input_get_input(t_lexer *lexer, t_fsm *fsm, t_input *input)
{
	char	c;

	c = get_next_char(lexer);
	if (fsm->input_state == N_INPUT)
		fsm_input_state_update(c, lexer, fsm);
	else if (fsm->input_state == IN_LINEBR)
		do_linebreak(lexer, "> ", fsm);
	else if (fsm->input_state == IN_SQUOTE)
		do_squote(lexer, fsm);
	else if (fsm->input_state == IN_DQUOTE)
		do_dquote(lexer, fsm);
	else if (fsm->input_state == IN_SUBSH)
		do_subsh(input, fsm);
}

char	*get_prompt_dir(void)
{
	char	path[264];

	if (getcwd(path, sizeof(path)))
	{
		return (ft_strdup(path));
	}
	else
		return (ft_strdup(""));
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


void	fsm_input_state_update(char c, t_lexer *lexer, t_fsm *fsm)
{
	if (c == '\0' && !lexer->input_len)
		fsm->state = COMPLETE;
	else if (c == '\0' && fsm->paren < 0)
	{
		fsm->input_state = INPUT_COMPLETE;
		fsm->state = ERROR;
	}
	else if (c == '\0' && fsm->paren > 0)
	{
		fsm->input_state = IN_SUBSH;
	}
	else if (c == '\0')
		fsm->input_state = INPUT_COMPLETE;
	if (c == '\\' && look_ahead(lexer) == '\0')
		fsm->input_state = IN_LINEBR;
	if (c == '\'')
		fsm->input_state = IN_SQUOTE;
	if (c == '"')
		fsm->input_state = IN_DQUOTE;
	if (c == '(')
	{
		fsm->input_state = IN_SUBSH;
		fsm->paren += 1;
		fsm->in_subshell = 1;
	}
	if(c == ')')
	{
		fsm->input_state = IN_SUBSH;
		fsm->paren -= 1;
	}
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

int	do_in_pipe(t_lexer *lexer, t_fsm *fsm)
{
	if (readline_pipe(lexer, "pipe> "))
		return (1);
	fsm->state = GET_INPUT;
	fsm->input_state = N_INPUT;
	return (0);
}

int	do_in_cmdand(t_lexer *lexer, t_fsm *fsm)
{
	if (readline_pipe(lexer, "cmdand> "))
		return (1);
	fsm->state = GET_INPUT;
	fsm->input_state = N_INPUT;
	return (0);
}

int	do_in_cmdor(t_lexer *lexer, t_fsm *fsm)
{
	if (readline_pipe(lexer, "cmdor> "))
		return (1);
	fsm->state = GET_INPUT;
	fsm->input_state = N_INPUT;
	return (0);
}

int	is_empty(char *str)
{
	int	i;

	i = 0;
	while (match_whitespace(str[i]))
		i++;
	if (str[i] == '\0')
		return (1);
	return (0);
}

void	do_squote(t_lexer *lexer, t_fsm *fsm)
{
	if (lexer->ch == '\0')
		readline_new_line(lexer, "squote> ");
	else if (lexer->ch == '\'' && fsm->in_subshell)
		fsm->input_state = IN_SUBSH;
	else if (lexer->ch == '\'')
		fsm->input_state = N_INPUT;
}

void	do_dquote(t_lexer *lexer, t_fsm *fsm)
{
	if (lexer->ch == '\0')
		readline_new_line(lexer, "dquote> ");
	else if (lexer->ch == '\"' && fsm->in_subshell)
		fsm->input_state = IN_SUBSH;
	else if (lexer->ch == '\"')
		fsm->input_state = N_INPUT;
}

void	do_subsh(t_input *input, t_fsm *fsm)
{
	t_lexer	*lexer;

	lexer = &input->lexer;
	if (fsm->paren < 0)
	{
		fsm->input_state = INPUT_COMPLETE;
		fsm->state = ERROR;
		input->unexpected = TOKEN_RPARENTHESIS;
	}
	else if (lexer->ch == '\0')
	{
		if (readline_no_new_line(lexer, "subsh> "))
		{
			fsm->input_state = INPUT_COMPLETE;
			fsm->state = ERROR;
			input->unexpected = TOKEN_END_OF_FILE;
			return ;
		}
	}
	else if (lexer->ch == '\'')
		fsm->input_state = IN_SQUOTE;
	else if (lexer->ch == '\"')
		fsm->input_state = IN_DQUOTE;
	else if (lexer->ch == ')' && fsm->paren > 0)
	{
		fsm->input_state = N_INPUT;
		fsm->paren -= 1;
	}
	else if (lexer->ch == ')')
	{
		fsm->input_state = N_INPUT;
		fsm->paren -= 1;
		fsm->in_subshell = 0;
	}
	else if (lexer->ch == '(')
	{
		fsm->paren += 1;
		fsm->input_state = N_INPUT;
	}
}

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
