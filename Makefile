# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: thi-le <thi-le@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/08/15 19:34:03 by thi-le            #+#    #+#              #
#    Updated: 2024/01/10 22:38:00 by thi-le           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv
INCLUDES = -I./includes/
SRCS_DIR = ./srcs/
OBJS_DIR = ./objs/
CC = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98 $(INCLUDES) -MMD -MP
DEBUG_FLAG = -g3
RM = rm -rf



SRC_FILES = main.cpp Ircserv.cpp


SRC = $(addprefix $(SRCS_DIR), $(SRC_FILES))
OBJ = $(addprefix $(OBJS_DIR), $(SRC_FILES:.cpp=.o))
DEP = $(OBJ:.o=.d)

all: $(NAME)

$(NAME): $(OBJ) Makefile
		@$(CC) $(CFLAGS) $(DEBUG_FLAG) $(OBJ) -o $(NAME)
		@echo "\033[32m$(NAME) created\033[0m"

$(OBJS_DIR)%.o: $(SRCS_DIR)%.cpp 
			@mkdir -p $(OBJS_DIR)
			@$(CC) $(CFLAGS) $(DEBUG_FLAG) -c $< -o $@
			@echo "\033[33m$@ object file created\033[0m"
-include $(DEP)

clean:
		@$(RM) $(OBJS_DIR)
		@echo "\033[31m$(NAME) objects deleted\033[0m"

fclean: clean
		@$(RM) $(NAME)
		@echo "\033[31m$(NAME) deleted\033[0m"

re: fclean all

.PHONY: all clean fclean re
