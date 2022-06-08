# PLACE AT THE TOP OF YOUR MAKEFILE
#---------------------------------
# Progress bar defs
#--------------------------------
#  words = count the number of words
ifneq ($(words $(MAKECMDGOALS)),1) # if no argument was given to make...
.DEFAULT_GOAL = all # set the default goal to all
#  http://www.gnu.org/software/make/manual/make.html
#  $@ = target name
#  %: = last resort recipe
#  --no-print-directory = don't print enter/leave messages for each output grouping
#  MAKEFILE_LIST = has a list of all the parsed Makefiles that can be found *.mk, Makefile, etc
#  -n = dry run, just print the recipes
#  -r = no builtin rules, disables implicit rules
#  -R = no builtin variables, disables implicit variables
#  -f = specify the name of the Makefile
%:
	@ $(MAKE) $@ --no-print-directory -rRf $(firstword $(MAKEFILE_LIST))
else
ifndef ECHO
#  execute a dry run of make, defining echo beforehand, and count all the instances of "COUNTTHIS"
T	:=	$(shell $(MAKE) $(MAKECMDGOALS) --no-print-directory \
		-nrRf $(firstword $(MAKEFILE_LIST)) \
		ECHO="COUNTTHIS" | grep -c "COUNTTHIS")
#  eval = evaluate the text and read the results as makefile commands
N	:=	x
#  Recursively expand C for each instance of ECHO to count more x's
C	=	$(words $N)$(eval N := x $N)
#  Multipy the count of x's by 100, and divide by the count of "COUNTTHIS"
#  Followed by a percent sign
#  And wrap it all in square brackets
ECHO = echo -ne "\r [`expr $C '*' 100 / $T`%]"
endif
#------------------
# end progress bar
#------------------

# REST OF YOUR MAKEFILE HERE

NAME			:= minishell
NAME_BONUS		:= minishell_bonus

CFILES			:=	builtins_utils.c	builtins.c	cleanup.c	error.c				execute_utils.c \
					execute.c 			init.c		io.c 		minishell.c			pipes.c \
					readline.c			signal.c 	wildcard.c	wildcard_utils.c \
					lib/get_next_line/get_next_line_utils.c		lib/get_next_line/get_next_line.c \
					lib/array.c			lib/math.c				lib/memory.c		lib/split.c \
					lib/str_utils.c		lib/write.c \
					parse/args_list.c	parse/cl_tok.c			parse/heredoc.c 	parse/lst_to_str.c \
					parse/parse_cmds.c	parse/parse_redir.c		parse/parse_utils.c	parse/parse.c \
					parse/print_utils.c

CFILES_BONUS	:=	builtins_utils.c	builtins.c	cleanup.c	error.c				execute_utils.c \
					execute.c 			init.c		io.c 		minishell.c			pipes.c \
					readline.c			signal.c 	wildcard.c	wildcard_utils.c \
					lib/get_next_line/get_next_line_utils.c		lib/get_next_line/get_next_line.c \
					lib/array.c			lib/math.c				lib/memory.c		lib/split.c \
					lib/str_utils.c		lib/write.c \
					parse/args_list.c	parse/cl_tok.c			parse/heredoc.c 	parse/lst_to_str.c \
					parse/parse_cmds.c	parse/parse_redir.c		parse/parse_utils.c	parse/parse.c \
					parse/print_utils.c

INC		= ./includes

SRC_DIR	= ./srcs
SRCS	= $(addprefix $(SRC_DIR)/, $(CFILES))

OBJ_DIR	= ./obj
OBJS	= $(addprefix $(OBJ_DIR)/, $(CFILES:.c=.o))

OBJ_DIR_BONUS	= ./obj_bonus
OBJS_BONUS		= $(addprefix $(OBJ_DIR_BONUS)/, $(CFILES_BONUS:.c=.o))

$(OBJ_DIR)/%.o:	$(SRC_DIR)/%.c
	@mkdir -p $(@D)
	@$(ECHO) Compiling $@
	@$(CC) $(CFLAGS) -o $@ -c $<

$(OBJ_DIR_BONUS)/%.o:	$(SRC_DIR)/%.c
	@mkdir -p $(@D)
	@$(ECHO) Compiling $@
	@$(CC) $(CFLAGS_BONUS) -o $@ -c $<

CC				= gcc
RM				= rm -rf
CFLAGS			= -Wall -Wextra -Werror -I./includes/ -I./readline-8.1 -g -D READLINE_LIBRARY=1
CFLAGS_BONUS	= -Wall -Wextra -Werror -I./includes/ -I./readline-8.1 -g -D READLINE_LIBRARY=1 -D BONUS=1

all:			$(NAME)
				@$(ECHO) Done

$(NAME):		$(OBJS)
				@cp .inputrc ~/
				@$(ECHO) Linking $@
				@$(CC) $(CFLAGS) -o $(NAME) $(OBJS) -Lreadline-8.1 -lreadline -lcurses

bonus:			$(NAME_BONUS)
				@$(ECHO) Done

$(NAME_BONUS):	$(OBJS_BONUS)
				@cp .inputrc ~/
				@$(ECHO) Linking $@
				@$(CC) $(CFLAGS_BONUS) -o $(NAME_BONUS) $(OBJS_BONUS) -Lreadline-8.1 -lreadline -lcurses

readline:
			@cd readline-8.1 && ./configure
			@make -sC readline-8.1/


clean:
				$(RM) $(OBJ_DIR) $(OBJ_DIR_BONUS)

fclean:			clean
				$(RM) $(NAME) $(NAME_BONUS) ~/.inputrc

re:				fclean $(NAME)

.PHONY:			all clean fclean re


#----- Progressbar endif at end Makefile
endif