/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   x_execution.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkilling <tkilling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/05 12:16:27 by tkilling          #+#    #+#             */
/*   Updated: 2023/04/24 11:18:02 by tkilling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef X_EXECUTION_H
# define X_EXECUTION_H

# include "ft_printf.h"
# include <stdio.h>
# include <sys/types.h>
# include <sys/wait.h>
# include "signals.h"

int				ft_command(char **str_arr, t_input *input, t_ast_node *root);
int				ft_cd(char **str_arr, t_input *input);
int				ft_pwd(void);;
int				ft_echo(char **str_arr, int fd);
int				ft_exit(char **str_arr, t_input *input);
int				ft_env(char **str_arr, t_input *input);
int				ft_export(char **str_arr, t_input *input);
int				ft_unset(char **str_arr, t_input *input);
int				ft_executable(char **str_arr, t_input *input);
int				ft_executable_no_env(char **str_arr, t_input *input);
char			*get_env_vars(t_arglist *list, t_input *input);
t_arglist		*expand_vars(char	*value);
t_arglist		*expand_vars_heredoc(char	*value);
void			expand_env_vars(char **args, t_input *input);
int				ft_execute(char *path, char **str_arr, t_input *input);
int				ft_execution(t_input *input, t_ast_node *root, int *fd);
int				ft_execute_tree(t_input *input, t_ast_node *root, int *fd, int subshell);
int				ft_pipe(t_input *input, t_ast_node *root, int *fd);
int				ft_and_if(t_input *input, t_ast_node *root, int *fd);
int				ft_or_if(t_input *input, t_ast_node *root, int *fd);
int				ft_subshell_no_tree(t_input *input, t_ast_node *root);
void			ft_change_tree(t_ast_node *root);
void			ft_change_tree_back(t_ast_node *root);
int				ft_add_subshell(t_input *input, t_ast_node *root, int *fd, int subshell);
int				ft_close_and_exit(int new_fd[2], int *fd, int status);

#endif
