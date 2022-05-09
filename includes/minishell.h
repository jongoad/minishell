#ifndef MINISHELL_H
# define MINISHELL_H

/* System includes */
# include <dirent.h>
# include <curses.h>
# include <term.h>
# include <stdlib.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/types.h>
# include <stdio.h>
# include <sys/stat.h>
# include <sys/ioctl.h>
# include <termios.h>
# include <signal.h>
# include <fcntl.h>
# include <string.h>
# include <sys/wait.h>
# include <sys/time.h>
# include <sys/resource.h>
# include <stdbool.h>
# include <errno.h>

/* User includes */
# include "enums.h"
# include "structs.h"

/* Macro defines */
# define PROMPT "minishell-> "

/* Error defines */
# define ERR_EXIT_ARGS_NUM ": too many arguments\n"
# define ERR_EXIT_NON_NUMERIC ": numeric argument required\n"

/* Function prototypes */


/* Builtin commands */
int		builtin_echo(t_shell *sh, t_cmd *cmd);
int		builtin_cd(t_shell *sh, t_cmd *cmd);
int		builtin_pwd(t_shell *sh, t_cmd *cmd);
int		builtin_export(t_shell *sh, t_cmd *cmd);
int		builtin_unset(t_shell *sh, t_cmd *cmd);
int		builtin_env(t_shell *sh, t_cmd *cmd);
int		builtin_exit(t_shell *sh, t_cmd *cmd);

/* Utility functions */
int		putchar_fd(char c, int fd);
int		putstr_fd(char *str, int fd);

/* Error handling */
int		msg_err_ret(int errnum, char *errname);
char	*get_err_msg(int errnum);


#endif