/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   x_execution.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/05 12:16:27 by tkilling          #+#    #+#             */
/*   Updated: 2023/04/26 13:24:43 by arobu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef X_EXECUTION_H
# define X_EXECUTION_H

# include "ft_printf.h"
# include <stdio.h>
# include <sys/types.h>
# include <sys/wait.h>
# include "signals.h"
# include <dirent.h>

typedef struct s_wild
{
	size_t			i;
	size_t			j;
	size_t			a;
	size_t			b;
	size_t			c_cp;
	DIR				*dir;
	struct dirent	*new_dir;
	size_t			*count;
}						t_wild;

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
int				ft_execute_tree(t_input *input, t_ast_node *root, int *fd);
int				ft_pipe(t_input *input, t_ast_node *root, int *fd);
int				ft_and_if(t_input *input, t_ast_node *root, int *fd);
int				ft_or_if(t_input *input, t_ast_node *root, int *fd);
int				ft_subshell_no_tree(t_input *input, t_ast_node *root);
void			ft_change_tree(t_ast_node *root);
void			ft_change_tree_back(t_ast_node *root);
int				ft_add_subshell(t_input *input, t_ast_node *root, \
					int *fd, int subshell);
int				ft_close_and_exit(int new_fd[2], int *fd, int status);
int				ft_redirect(t_ast_node *root, int *stdin_cp, int *stdout_cp);
void			ft_redirect_back(int *stdin_cp, int *stdout_cp);
int				is_directory(const char *path);
int				ft_put_error(char **str_arr, char *str, int status);
int				ft_put_error_n(char **str_arr, char *str, int status);
int				ft_directory_error(char **str_arr, int *status);
void			ft_cmd_not_found(char **str_arr, int *status);
int				open_file(char **filename, int io, t_input *input);
void			ft_check(char *str, char **arr, \
					char new_arr[1024][256], size_t *count);
char			**ft_wildcard(char **str_arr, t_ast_node *root);
void			ft_get_arr(char *str, char new_arr[1024][256], size_t *count);
#endif
