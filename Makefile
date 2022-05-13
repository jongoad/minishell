NAME			:= minishell

SRCS			:=	./srcs/builtins_utils.c ./srcs/builtins.c ./srcs/cleanup.c ./srcs/error.c ./srcs/execute_utils.c \
					./srcs/execute.c ./srcs/init.c ./srcs/io.c ./srcs/minishell.c ./srcs/pipes.c ./srcs/readline.c \
					./srcs/lib/get_next_line/get_next_line_utils.c ./srcs/lib/get_next_line/get_next_line.c \
					srcs/lib/array.c srcs/lib/math.c srcs/lib/memory.c srcs/lib/split.c \
					srcs/lib/str_utils.c srcs/lib/write.c \
					srcs/parse/args_list.c srcs/parse/cmd_utils.c srcs/parse/get_cl_tok.c srcs/parse/parse_redir.c \
					srcs/parse/parse_utils.c srcs/parse/parse.c srcs/parse/print_utils.c 

OBJS			:= $(SRCS:.c=.o)

CC				= gcc
RM				= rm -f
CFLAGS			= -Wall -Wextra -Werror -I./includes/

all:			$(NAME)

$(NAME):		$(OBJS)
				$(CC) $(CFLAGS) -o $(NAME) $(OBJS) -lreadline

clean:
				$(RM) $(OBJS)

fclean:			clean
				$(RM) $(NAME)

re:				fclean $(NAME)

.PHONY:			all clean fclean re

