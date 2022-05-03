#include "../includes/minishell.h"

/* Print error message and return errnum */
int	msg_err_ret(int errnum, char *errname)
{
	errno = errnum;
	putstr_fd("minishell: ", 2);
	perror(errname);
	return (errnum);
}
