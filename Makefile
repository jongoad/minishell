SRCS			:= ./srcs/builtins_utils.c ./srcs/builtins.c ./srcs/debug.c ./srcs/error.c ./srcs/execute_utils.c \
				   ./srcs/execute.c ./srcs/init.c ./srcs/memory.c ./srcs/minishell.c ./srcs/pipes.c \
				   ./srcs/utils.c ./srcs/split.c

OBJS			:= $(SRCS:.c=.o)

CC				= gcc
RM				= rm -f
CFLAGS			= -Wall -Wextra -Werror -I./includes/

NAME			= minishell

all:			$(NAME)

$(NAME):		$(OBJS)
				$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

clean:
				$(RM) $(OBJS)

fclean:			clean
				$(RM) $(NAME)

re:				fclean $(NAME)

.PHONY:			all clean fclean re