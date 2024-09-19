# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aklimchu <aklimchu@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/24 11:18:05 by aklimchu          #+#    #+#              #
#    Updated: 2024/09/19 11:00:24 by aklimchu         ###   ########.fr        #
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
CC 			= cc
CFLAGS		= -Wall -Wextra -Werror -lpthread
RM			= rm -f

# Source / OBJ files / Includes
SRC 		= 
OBJ 		= $(SRC:.c=.o)
INCLUDE		= -I "./inc"

# Rules
all:		$(NAME)

%.o: %.c 
	$(CC) $(CFLAGS) $(INCLUDE) $^ -c -o $@

$(NAME):	$(OBJ)
	@$(CC) $(CFLAGS) $(OBJ) $(INCLUDE) -o $(NAME)
	@echo "$(GREEN)SUCCESS, MINISHELL IS READY $(DEF_COLOR)"

clean:
	@echo "$(GREEN)Deleting object files... $(DEF_COLOR)"
	$(RM) $(OBJ)

fclean:		clean
	@echo "$(GREEN)Deleting minishell... $(DEF_COLOR)"
	$(RM) $(NAME)
	@echo "$(GREEN)CLEAR $(DEF_COLOR)"

re: 		fclean all

.PHONY: 	all clean fclean re
