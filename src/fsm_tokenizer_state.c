/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fsm_tokenizer_state.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/23 21:17:12 by arobu             #+#    #+#             */
/*   Updated: 2023/04/23 21:29:04 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"
#include <sys/wait.h>
#include "signals.h"

int	is_running(t_fsm *fsm);

void	tokenize(t_input *input, t_fsm *fsm)
{
	t_lexer			*lexer;
	t_token			*token;
	t_token_list	*tokens;

	if (init_state_get_tokens_substates(fsm))
		return ;
	init_state_get_tokens(&lexer, &tokens, &token, input);
	while (is_running(fsm))
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

int	is_running(t_fsm *fsm)
{
	return (fsm->tok_state != TOK_COMPLETE && \
			fsm->state != INCOMPLETE && \
			fsm->state != COMPLETE && \
			fsm->state != ERROR);
}

void	init_state_get_tokens(t_lexer **lexer, t_token_list **tokens, \
			t_token **token, t_input *input)
{

	*lexer = &input->lexer;
	*token = NULL;
	*tokens = input->tokens;
}

int	init_state_get_tokens_substates(t_fsm *fsm)
{
	if (fsm->state == ERROR)
		return (1);
	fsm->state = GET_TOKENS;
	fsm->tok_state = N_TOKENIZER;
	return (0);
}
