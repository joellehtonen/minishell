# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jlehtone <jlehtone@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/24 11:18:05 by aklimchu          #+#    #+#              #
#    Updated: 2024/10/02 16:46:25 by jlehtone         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #



#Colors
DEF_COLOR = \033[0;39m
GRAY = \033[0;90m
RED = \033[0;91m
GREEN = \033[0;92m
YELLOW = \033[0;93m
BLUE = \033[0;94m
MAGENTA = \033[0;95m
CYAN = \033[0;96m
WHITE = \033[0;97m

# Program name
NAME		= minishell

# Compiler
CC 			= gcc
CFLAGS		= -ggdb3 -Wall -Werror -Wextra -g -I $(LIBFT_DIR)

RM			= rm -f
RMDIR		= rm -rf

# Libft
LIBFT_DIR	= libft
LIBFT_LIB	= $(LIBFT_DIR)/libft.a

# Source / OBJ files / Includes
SRC 		= ./src/main.c \
			./src/envp/envp_tools.c \
			./src/envp/list_tools_envps.c \
			./src/envp/ft_split_list.c \
			./src/parsing/list_tools_tokens.c \
			./src/parsing/list_tools_tokens_delete.c \
			./src/parsing/read_input.c \
			./src/parsing/input_parser.c \
			./src/parsing/check_syntax.c \
			./src/parsing/assign_values.c \
			./src/parsing/parser_utils.c \
			./src/builtins/builtins_extra.c \
			./src/builtins/builtins_tools.c \
			./src/builtins/builtins.c \
			./src/builtins/cd.c \
			./src/builtins/echo.c \
			./src/str_join_four.c \
			./src/printing_tools.c \
			./src/signals.c \
			./src/exec/execute.c \
			./src/exec/child_process.c \
			./src/exec/exec_tools.c \
			./src/exec/exec_tools_extra.c \
			./src/exec/param_and_path.c \
			./src/exec/pipe_and_fork.c \
			./src/exec/free_exec.c \
			./src/exec/here_doc.c \
			./src/exec/here_doc_tools.c \
			./src/exit.c
OBJ 		= $(SRC:%.c=$(OBJ_DIR)/%.o)
OBJ_DIR		= obj
INCLUDE		= -I "./inc"

# Rules
all:		$(NAME)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(NAME): $(OBJ_DIR) $(OBJ)
	@echo "$(GREEN)Compiling libft... $(DEF_COLOR)"
	@make -C $(LIBFT_DIR)  --no-print-directory		# make libft
	@cp $(LIBFT_LIB) $(NAME)	# copy libft to current
	@$(CC) $(CFLAGS) $(OBJ) -lreadline $(LIBFT_LIB) $(INCLUDE) -o $(NAME)
	@echo "$(GREEN)SUCCESS, MINISHELL IS READY $(DEF_COLOR)"

clean:
	@echo "$(GREEN)Deleting object files... $(DEF_COLOR)"
	$(RMDIR) $(OBJ_DIR)
	@make clean -C $(LIBFT_DIR) --no-print-directory

fclean:		clean 
	@echo "$(GREEN)Deleting minishell... $(DEF_COLOR)"
	$(RM) $(NAME) $(LIBFT_LIB)
	@make fclean -C $(LIBFT_DIR) --no-print-directory
	@echo "$(GREEN)CLEAR $(DEF_COLOR)"

re: 		fclean all

.PHONY: 	all clean fclean re
