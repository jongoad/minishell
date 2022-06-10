NAME			:= ms_tester

SRCS			:= tester.c
OBJS			:= $(SRCS:.c=.o)

CC				:= gcc
CFLAGS			:= -I.
RM				:= rm -rf

all:			$(NAME)

$(NAME):		$(OBJS)
				$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

clean:
				$(RM) $(OBJS)
				

fclean:			clean
				$(RM) $(NAME)

re:				fclean all

.PHONY:			all clean fclean re