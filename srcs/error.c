#include "../includes/minishell.h"

/* Print error message and return errnum */
int	msg_err_ret(int errnum, char *errname)
{
	errno = errnum;
	putstr_fd("minishell: ", 2);
	perror(errname);
	return (errnum);
}

/* Write bash-style error messages to STDERR */
void	put_err_msg(char *sh_name, char *cmd, char *arg, char *msg)
{
	if (sh_name)
	{
		putstr_fd(sh_name, 2);
		putstr_fd(": ", 2);
	}
	if (cmd)
	{
		putstr_fd(cmd, 2);
		putstr_fd(": ", 2);
	}
	if (arg)
	{
		putstr_fd("'", 2);
		putstr_fd("arg", 2);
		putstr_fd("'", 2);
		putstr_fd(": ", 2);
	}
	if (msg)
	{
		putstr_fd(msg, 2);
		putstr_fd("\n", 2);
	}
}
