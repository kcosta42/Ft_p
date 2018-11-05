# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kcosta <kcosta@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/08/16 19:08:49 by kcosta            #+#    #+#              #
#    Updated: 2018/11/05 15:57:23 by kcosta           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# ========== Editable ========== #
SERVER	:= server
CLIENT	:= client
# ============================== #

# ========== Standard ========== #
CC		:= gcc
FLAGS	:= -Wall -Wextra -Werror -g #-fsanitize=address
# ============================== #

# =========== Files ============ #
SERVER_SRCS_FILES	:=	server.c			\
						server_pi.c			\
						server_pi_utils.c	\
						server_init.c		\
						server_utils.c		\
						command_pwd.c		\
						command_cd.c		\
						command_cd_utils.c	\
						command_del.c		\
						command_get.c		\
						command_put.c
CLIENT_SRCS_FILES	:=	client.c			\
						client_pi.c			\
						client_dtp.c		\
						lcommand_cd.c		\
						lcommand_cd_utils.c	\
						lcommand_put.c

SERVER_HEADERS_FILES	:=	server.h
CLIENT_HEADERS_FILES	:=	client.h
# ============================== #

# ========== Sources =========== #
SERVER_PATH	:= sources/server/
CLIENT_PATH	:= sources/client/
SERVER_SRCS	:= $(addprefix $(SERVER_PATH), $(SERVER_SRCS_FILES))
CLIENT_SRCS	:= $(addprefix $(CLIENT_PATH), $(CLIENT_SRCS_FILES))
# ============================== #

# ========== Objects =========== #
OBJS_PATH			:= objs/
SERVER_OBJS_PATH	:= objs/server/
CLIENT_OBJS_PATH	:= objs/client/
SERVER_OBJS			:= $(addprefix $(SERVER_OBJS_PATH), $(SERVER_SRCS_FILES:.c=.o))
CLIENT_OBJS 		:= $(addprefix $(CLIENT_OBJS_PATH), $(CLIENT_SRCS_FILES:.c=.o))
# ============================== #

# ========== Includes ========== #
INCLUDES_PATH	:= includes/
INCLUDES		:= -I $(INCLUDES_PATH)
SERVER_HEADERS	:= $(addprefix $(INCLUDES_PATH), $(SERVER_HEADERS_FILES))
CLIENT_HEADERS	:= $(addprefix $(INCLUDES_PATH), $(CLIENT_HEADERS_FILES))
# ============================== #

# ========== Libraries ========= #
LIBFT_PATH		:= libft/
LIBFT_INCLUDES	:= -I libft/includes
LIBFT			:= -L $(LIBFT_PATH) -lft
# ============================== #

.PHONY: all libft clean fclean re

all: libft $(SERVER) $(CLIENT)

libft:
	@make -C $(LIBFT_PATH)

$(SERVER_OBJS_PATH)%.o: $(SERVER_PATH)%.c $(SERVER_HEADERS) $(LIBFT_PATH)libft.a
	@mkdir $(OBJS_PATH) 2> /dev/null || true
	@mkdir $(SERVER_OBJS_PATH) 2> /dev/null || true
	$(CC) $(FLAGS) $(INCLUDES) $(LIBFT_INCLUDES) -o $@ -c $<

$(CLIENT_OBJS_PATH)%.o: $(CLIENT_PATH)%.c $(CLIENT_HEADERS) $(LIBFT_PATH)libft.a
	@mkdir $(OBJS_PATH) 2> /dev/null || true
	@mkdir $(CLIENT_OBJS_PATH) 2> /dev/null || true
	$(CC) $(FLAGS) $(INCLUDES) $(LIBFT_INCLUDES) -o $@ -c $<

$(SERVER): $(SERVER_OBJS)
	$(CC) $(FLAGS) $(LIBFT) $(SERVER_OBJS) -o $@

$(CLIENT): $(CLIENT_OBJS)
	$(CC) $(FLAGS) $(LIBFT) $(CLIENT_OBJS) -o $@

clean:
	@make -C $(LIBFT_PATH) clean &> /dev/null
	@rm -fv $(SERVER_OBJS) $(CLIENT_OBJS)
	@rmdir $(OBJS_PATH) 2> /dev/null || true

fclean: clean
	@make -C $(LIBFT_PATH) fclean &> /dev/null
	@rm -fv $(SERVER) $(CLIENT)

re: fclean all
