/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_analyzer.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 13:43:59 by arobu             #+#    #+#             */
/*   Updated: 2023/03/29 22:28:39 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	analyze_syntax(t_token_list *tokens, int *unexpected)
{
	t_fsm	fsm;
	t_token	*token;

	token = tokens->first;
	fsm.state = IN_PREFIX;
	while (token->type != TOKEN_EOF)
	{
		if (fsm.state == NOTHING)
		{
			if (is_prefix(token))
				fsm.state = IN_PREFIX;
			else if (is_cmd_word(token))
				fsm.state = IN_CMD;
			else if (is_cmd_suffix(token))
				fsm.state = IN_SUFFIX;
			else if (token->type == TOKEN_LPARENTHESIS)
				fsm.state = IN_SUBSHELL;
			else if (token->type == TOKEN_PIPE)
				fsm.state = IN_PIPE;
			else if (token->type == IN_AND_IF)
				fsm.state = IN_AND_IF;
			else if (token->type == IN_OR_IF)
				fsm.state = IN_OR_IF;
		}
		print_token_value(token);
		if (fsm.state == IN_PREFIX)
		{
			if (accept_prefix(&token, &fsm) == TOKEN_UNEXPECTED)
			{
				return (*unexpected = token->next->type);
			}
		}
	}
	return (0);
}

int	accept_prefix(t_token **token, t_fsm *state)
{
	if (is_input_redir(*token) || is_output_redir(*token))
	{
		(*token) = (*token)->next;
		if (is_cmd_word(*token))
		{
			(*token) = (*token)->next;
			return (0);
		}
	}
	else if (is_assign_word(*token))
	{
		(*token) = (*token)->next;
		return (0);
	}
	else if (is_cmd_word(*token))
	{
		state->state = IN_CMD;
		return (0);
	}
	return (TOKEN_UNEXPECTED);
}

// void	token_next(t_token **token);
// int		is_correct_syntax_word(t_token **token);
// int		is_correct_syntax_redirection(t_token **token);
// int		is_correct_syntax_pipe_operator(t_token **token, int *first);
// int		is_correct_first_word(t_token *token, int *first);
// int		is_parenthesis(t_token **token);

// int	analyze_syntax(t_token_list *tokens, int *unexpected)
// {
// 	t_token			*token;
// 	int				first;

// 	token = tokens->first;
// 	first = 0;
// 	while (token && token->type != TOKEN_EOF)
// 	{
// 		if (is_correct_first_word(token, &first))
// 			return (*unexpected = token->type);
// 		if (is_correct_syntax_word(&token) == TOKEN_UNEXPECTED)
// 			return (*unexpected = token->type);
// 		else if (is_correct_syntax_redirection(&token))
// 			return (*unexpected = token->type);
// 		else if (is_correct_syntax_pipe_operator(&token, &first))
// 			return (*unexpected = token->type);
// 		else if (token->type == TOKEN_LPARENTHESIS)
// 		{
// 			token = token->next;
// 			if (!is_valid_beginning(token))
// 				return (*unexpected = token->type);
// 		}
// 		else if (token->type == TOKEN_RPARENTHESIS)
// 		{
// 			token = token->next;
// 			if (!is_pipe(token) || !is_logical_op(token) || token->type != TOKEN_EOF)
// 				return (*unexpected == token->type);
// 		}
// 	}
// 	return (0);
// }


// int	is_correct_first_word(t_token *token, int *first)
// {
// 	if (!(*first) && !is_valid_beginning(token))
// 		return (1);
// 	*first = 1;
// 	return (0);
// }

// int	is_correct_syntax_word(t_token **token)
// {
// 	while (is_cmd_word(*token) || (*token)->type == TOKEN_ASSIGN_WORD)
// 		(*token) = (*token)->next;
// 	if (is_input_redir(*token) || is_output_redir(*token) || is_pipe(*token) || \
// 		is_logical_op(*token) || (*token)->type == TOKEN_EOF)
// 		return (1);
// 	else if ((*token)->type == TOKEN_RPARENTHESIS && \
// 				((*token)->next->type == TOKEN_EOF || \
// 					is_pipe((*token)->next) || is_logical_op((*token)->next)))
// 		return (1);
// 	(*token) = (*token)->next;
// 	return (TOKEN_UNEXPECTED);
// }

// int	is_correct_syntax_redirection(t_token **token)
// {
// 	if (is_redirection(*token))
// 	{
// 		if (is_type_word(token) == TOKEN_UNEXPECTED)
// 			return (1);
// 	}
// 	return (0);
// }

// int	is_correct_syntax_pipe_operator(t_token **token, int *first)
// {
// 	if (is_pipe(*token) || is_logical_op(*token))
// 	{
// 		if (is_correct_pipe_logicalop(token, first) == TOKEN_UNEXPECTED)
// 			return (1);
// 		*first = 0;
// 	}
// 	return (0);
// }

// void token_next(t_token **token)
// {
// 	*token = (*token)->next;
// }

// int	is_redirection(t_token *token)
// {
// 	return (token->type == TOKEN_LESS || token->type == TOKEN_GREAT || \
// 		token->type == TOKEN_DLESS || token->type == TOKEN_DGREAT);
// }

// int	is_valid_beginning(t_token *token)
// {
// 	return (is_cmd_word(token) || \
// 			is_input_redir(token) || \
// 				is_output_redir(token) || \
// 					token->type == TOKEN_LPARENTHESIS || \
// 						token->type == TOKEN_ASSIGN_WORD);
// }

// int	is_correct_pipe_logicalop(t_token **token, int *first)
// {
// 	(*token) = (*token)->next;
// 	if (is_prefix((*token)) || is_cmd_word((*token)) || is_cmd_suffix(*token)
// 		|| (*token)->type == TOKEN_LPARENTHESIS)
// 		return (1);
// 	return (TOKEN_UNEXPECTED);
// }

// int	is_pipe(t_token *token)
// {
// 	return (token->type == TOKEN_PIPE);
// }

// int	is_logical_op(t_token *token)
// {
// 	return (token->type == TOKEN_AND_IF || token->type == TOKEN_OR_IF);
// }

// int	is_type_word(t_token **token)
// {
// 	(*token) = (*token)->next;
// 	if ((*token)->type == TOKEN_WORD)
// 	{
// 		(*token) = (*token)->next;
// 		return (1);
// 	}
// 	else if ((*token)->type == TOKEN_SQUOTE)
// 	{
// 		(*token) = (*token)->next;
// 		return (1);
// 	}
// 	else if ((*token)->type == TOKEN_DQUOTE)
// 	{
// 		(*token) = (*token)->next;
// 		return (1);
// 	}
// 	return (TOKEN_UNEXPECTED);
// }
