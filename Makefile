# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/06 16:57:34 by dwimpy            #+#    #+#              #
#    Updated: 2023/03/18 13:49:28 by arobu            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Variables
NAME			= minishell
INCLUDE			= -I include/ -I libft-printf/include/ -I get_next_line/gnl/include
DSYM			= ./minishell.dSYM
SRC_DIR			= ./src
OBJ_DIR			= ./obj
MAIN_FILE		:= main.c
NORM_INCLUDE	= ./include
LIBFT_FOLDER	:= ./libft-printf
GNL				:= ./get_next_line

# Compiler
CC			= cc
LDLFLAGS	= -lreadline -lft -L./libft-printf
CFLAGS		= -g3 #-Wall -Werror -Wextra -g3 -pthread 

ASAN		= #-fsanitize=address

#Archive and Remove
RM			= rm -f
AR			= ar rcs

# Colors
DEF_COLOR = \033[0;39m
RED = \033[0;91m
GREEN = \033[0;92m
YELLOW = \033[0;93m
BLUE = \033[0;94m
MAGENTA = \033[0;95m
CYAN = \033[0;96m
WHITE = \033[0;97m

# Sources
SRCS	:=	$(wildcard $(SRC_DIR)/*.c)
# Objects
OBJS	:= 	$(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

# Rules
all:	libft $(NAME)

$(NAME): $(OBJS) $(MAIN_FILE)
	@$(CC) $(INCLUDE) $(FRAMEWORK) $(ASAN) $(OBJS) $(MAIN_FILE) -o $@ $(LDLFLAGS)
	@echo "$(YELLOW)minishell$(DEF_COLOR) $(CYAN)done.$(DEF_COLOR)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@ echo "$(MAGENTA)Compiling:$(DEF_COLOR) $<."
	@ $(CC) $(INCLUDE) $(CFLAGS) -c $< -o $@ 

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

libft:
			@make -C $(LIBFT_FOLDER)

 clean:
			@$(RM) -rdf $(OBJ_DIR)
			@$(RM) -rdf $(DSYM)
			@echo "$(YELLOW)minishell$(DEF_COLOR) $(CYAN)successfully cleaned!$(DEF_COLOR)"

fclean:		clean
			@make fclean -C $(LIBFT_FOLDER)
			@$(RM) -f $(NAME)
			@echo "$(YELLOW)All$(DEF_COLOR) $(CYAN)objects successfully cleaned!$(DEF_COLOR)"

re:			fclean all


bonus:	libft	mlx	$(NAME)

.PHONY:		all mlx relibft libft clean fclean re norm
