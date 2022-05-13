#include "./parse_test.h"

void	ft_putstr_fd(char *str, int fd)
{
	if (!str)
		return ;
	write(fd, str, ft_strlen(str));
	return ;
}

int	is_set(char const c, char const *set)
{
	if (!set || !c)
		return (0);
	while (*set)
	{
		if (*set == c)
			return (1);
		set++;
	}
	return (0);
}

// If c was found in the set, returns its index.
// If not found, returns -1
int	is_set_ret(char const c, char const *set)
{
	int	i;

	i = 0;
	while (set && set[i])
	{
		if (set[i] == c)
			return (i);
		i++;
	}
	return (-1);
}

void	ft_bzero(void *s, size_t n)
{
	unsigned char	*ptr;

	ptr = s;
	while (n--)
		*ptr++ = 0;
}

void	free_split(char **split)
{
	int	i;

	if (!split || !*split)
		return ;
	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split[i]);
	free(split);
	return ;
}