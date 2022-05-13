#include "minishell.h"

/* Write a character to specified file descriptor */
int	putchar_fd(char c, int fd)
{
	write(fd, &c, sizeof(c));
	return (sizeof(c));
}

/* Write a string to specified file descriptor */
int	putstr_fd(char *str, int fd)
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

/* Write a string to specified file descriptor */
int	putendl_fd(char *str, int fd)
{
	int count;

	count = 0;
	while (*str)
	{
		count += putchar_fd(*str, fd);
		str++;
	}
	count += putstr_fd("\n", fd);
	return (count);
}
