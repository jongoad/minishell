NAME			:= minishell

CFILES			:=	builtins_utils.c	builtins.c	cleanup.c	error.c				execute_utils.c \
					execute.c 			init.c		io.c 		minishell.c			pipes.c \
					readline.c			signal.c \
					lib/get_next_line/get_next_line_utils.c		lib/get_next_line/get_next_line.c \
					lib/array.c			lib/math.c				lib/memory.c		lib/split.c \
					lib/str_utils.c		lib/write.c \
					parse/args_list.c	parse/cmd_utils.c		parse/cl_tok.c		parse/heredoc.c \
					parse/lst_to_str.c	parse/parse_redir.c		parse/parse_utils.c	parse/parse.c \
					parse/print_utils.c

INC		= ./includes

SRC_DIR	= ./srcs
SRCS	= $(addprefix $(SRC_DIR)/, $(CFILES))

OBJ_DIR	= ./obj
OBJS	= $(addprefix $(OBJ_DIR)/, $(CFILES:.c=.o))

$(OBJ_DIR)/%.o:	$(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -o $@ -c $<

CC				= gcc
RM				= rm -rf
CFLAGS			= -Wall -Wextra -Werror -I./includes/ -I./readline-8.1 -g -D READLINE_LIBRARY=1

all:			$(NAME)

$(NAME):		$(OBJS)
				cp .inputrc ~/
				$(CC) $(CFLAGS) -o $(NAME) $(OBJS) -Lreadline-8.1 -lreadline -lcurses


clean:
				$(RM) $(OBJ_DIR)

fclean:			clean
				$(RM) $(NAME) ~/.inputrc

re:				fclean $(NAME)

.PHONY:			all clean fclean re

