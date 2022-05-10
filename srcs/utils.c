#include "../includes/minishell.h"

int putchar_fd(char c, int fd)
{
	write(fd, &c, sizeof(c));
	return (sizeof(c));
}

int putstr_fd(char *str, int fd)
{
	int count;

	count = 0;
	while (*str)
	{
		count += putchar_fd(*str, fd);
		str++;
	}
	return (count);
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

/* Count the number of elements in a 2d array that is null terminated */
int	count_array(void **array)
{
	int count;

	count = 0;
	while (array[count])
		count++;
	return (count);
}