#ifndef ERROR_H
# define ERROR_H

#include "minishell.h"

/* Error defines */
# define ERR_EXIT_ARGS_NUM "too many arguments"
# define ERR_EXIT_NON_NUMERIC "numeric argument required"
# define ERR_PIPE "unable to allocate pipes"
# define ERR_IDENTIFER	"not a valid identifier" 
# define ERR_CMD "command not found"
# define ERR_FILE "no such file or directory"
# define ERR_PWD "error retrieving current directory"
# define ERR_CWD "getcwd: cannot access parent directories"
# define ERR_ACCESS "permission denied"
/* Bash errors */
typedef enum e_bash_errors
{
	E_CMDNOTFOUND = 127
} t_bash_errors;

#endif