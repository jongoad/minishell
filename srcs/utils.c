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
