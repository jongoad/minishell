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

#--------------------------------#
#    Name and file information   #
#--------------------------------#
NAME			:= minishell
NAME_BONUS		:= minishell_bonus


CFILES			:=	builtins/builtins_utils.c	builtins/builtins_utils_2.c \
					builtins/builtins.c			builtins/builtins_2.c \
					parse/add_token.c			parse/args_list.c		parse/tokenizer.c		parse/heredoc.c 		parse/lst_to_str.c \
					parse/parse_cmds.c			parse/parse_redir.c		parse/parse_token.c		parse/parse_utils.c		parse/parse.c \
					parse/print_utils.c  \
					lib/gnl/gnl_utils.c			lib/gnl/gnl.c			lib/array.c				lib/math.c \
					lib/memory.c				lib/split.c				lib/str_utils.c			lib/str_utils_2.c \
					lib/str_utils_3.c			lib/str_utils_4.c		lib/write.c \
					minishell.c					readline.c				init.c					init_2.c \
					io.c						pipes.c					execute.c				execute_utils.c	\
					signal.c					error.c					cleanup.c				cleanup_2.c				cleanup_3.c \


CFILES_BONUS	:=	builtins/builtins_utils_bonus.c	builtins/builtins_utils_2_bonus.c \
					builtins/builtins_bonus.c		builtins/builtins_2_bonus.c \
					wildcard/wildcard_bonus.c		wildcard/wildcard_2_bonus.c \
					wildcard/wildcard_utils_bonus.c	\
					parse/add_token_bonus.c			parse/args_list_bonus.c		parse/tokenizer_bonus.c		parse/heredoc_bonus.c 		parse/lst_to_str_bonus.c \
					parse/parse_cmds_bonus.c		parse/parse_redir_bonus.c	parse/parse_token_bonus.c	parse/parse_utils_bonus.c	parse/parse_bonus.c \
					parse/print_utils_bonus.c		parse/validate_cli_bonus.c  parse/parse_jobs.c			parse/job_lst_utils.c \
					lib/gnl/gnl_utils_bonus.c		lib/gnl/gnl_bonus.c			lib/array_bonus.c			lib/math_bonus.c \
					lib/memory_bonus.c				lib/split_bonus.c			lib/str_utils_bonus.c		lib/str_utils_2_bonus.c \
					lib/str_utils_3_bonus.c			lib/str_utils_4_bonus.c		lib/write_bonus.c \
					minishell_bonus.c				readline_bonus.c			init_bonus.c				init_2_bonus.c \
					io_bonus.c						pipes_bonus.c				execute_bonus.c				execute_utils_bonus.c	\
					signal_bonus.c					error_bonus.c				cleanup_bonus.c				cleanup_2_bonus.c 			cleanup_3_bonus.c \

#---------------------------------------------------------#
#   Directory information and object directory building   #
#---------------------------------------------------------#

INC				:= ./includes
INC_BONUS		:= ./includes_bonus

SRC_DIR			= ./srcs
SRCS			= $(addprefix $(SRC_DIR)/, $(CFILES))

SRC_DIR_BONUS	= ./srcs_bonus
SRCS_BONUS		= $(addprefix $(SRC_DIR_BONUS)/, $(CFILES_BONUS))

OBJ_DIR			= ./obj
OBJS			= $(addprefix $(OBJ_DIR)/, $(CFILES:.c=.o))

OBJ_DIR_BONUS	= ./obj_bonus
OBJS_BONUS		= $(addprefix $(OBJ_DIR_BONUS)/, $(CFILES_BONUS:.c=.o))

$(OBJ_DIR)/%.o:	$(SRC_DIR)/%.c
	@mkdir -p $(@D)
	@$(ECHO) Compiling $@
	@$(CC) $(CFLAGS) -o $@ -c $<

$(OBJ_DIR_BONUS)/%.o:	$(SRC_DIR_BONUS)/%.c
	@mkdir -p $(@D)
	@$(ECHO) Compiling $@
	@$(CC) $(CFLAGS_BONUS) -o $@ -c $<

#--------------------------------#
#  Compiler settings and flags   #
#--------------------------------#

CC				= gcc
RM				= rm -rf
CFLAGS			= -Wall -Wextra -Werror -I$(INC) -I./readline-8.1 -g -D READLINE_LIBRARY=1
CFLAGS_BONUS	= -Wall -Wextra -Werror -I$(INC_BONUS) -I./readline-8.1 -g -D READLINE_LIBRARY=1

#--------------------------------#
#   Makefile rules and targets   #
#--------------------------------#

all:			readline $(NAME)
				@$(ECHO) Done

$(NAME):		$(OBJS)
				@cp .inputrc ~/
				@$(ECHO) Linking $@
				@$(CC) $(CFLAGS) -o $(NAME) $(OBJS) -Lreadline-8.1 -lreadline -lcurses

bonus:			readline $(NAME_BONUS)
				@$(ECHO) Done

$(NAME_BONUS):	$(OBJS_BONUS)
				@cp .inputrc ~/
				@$(ECHO) Linking $@
				@$(CC) $(CFLAGS_BONUS) -o $(NAME_BONUS) $(OBJS_BONUS) -Lreadline-8.1 -lreadline -lcurses

readline:
ifeq (,$(wildcard ./readline-8.1/is_configured))
	@cd readline-8.1 && ./configure --silent && make -s && touch is_configured
endif

clean:
				$(RM) $(OBJ_DIR) $(OBJ_DIR_BONUS)

clean_readline:
				$(RM) readline-8.1/is_configured
				echo "Cleaning readline-8.1 obj files"
				@make clean -sC readline-8.1

fclean:			clean
				$(RM) $(NAME) $(NAME_BONUS) ~/.inputrc

re:				fclean $(NAME)

.PHONY:			all clean fclean re


#----- Progressbar endif at end Makefile
endif