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

/* Count the number of elements in a 2d array that is null terminated */
int	count_array(void **array)
{
	int count;

	count = 0;
	while (array && array[count])
		count++;
	return (count);
}

/* Return the absolute value of an integer */
int	abs_val(int n)
{
	if (n < 0)
		return (-n);
	return (n);
}

/* Return a malloced copy of a strin */
char	*ft_strdup(const char *s1)
{
	unsigned int	i;
	char			*str;

	i = 0;
	str = (char *)malloc(sizeof(char) * (ft_strlen(s1) + 1));
	if (!str)
	{
		errno = ENOMEM;
		return (NULL);
	}
	while (s1[i])
	{
		str[i] = s1[i];
		i++;
	}
	str[i] = '\0';
	return (str);
}

/* Compare n values of two strings and return result */
int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	unsigned int	i;

	i = 0;
	while ((s1[i] || s2[i]) && i < n)
	{
		if ((unsigned char)s1[i] != (unsigned char)s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return (0);
}

/* Join together two string and return result */
char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*str;
	char	*p_str;

	if (!s1 || !s2)
		return (NULL);
	str = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2)) + 1);
	if (!str)
		return (NULL);
	p_str = str;
	while (*s1)
	{
		*p_str = *s1;
		p_str++;
		s1++;
	}
	while (*s2)
	{
		*p_str = *s2;
		p_str++;
		s2++;
	}
	*p_str = '\0';
	return (str);
}

/* Return the length of a string */
size_t	ft_strlen(const char *s)
{
	unsigned int	i;

	i = 0;
	while (s[i] != '\0')
		i++;
	return (i);
}

/* Set memory to a specified value */
void	*ft_memset(void *b, int c, size_t len)
{
	unsigned char	*p;

	p = (unsigned char *)b;
	while (len--)
		*p++ = c;
	return (b);
}

/* Convert a string to lower case */
char	*str_to_lower(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] >= 'A' && str[i] <= 'Z')
			str[i] = str[i] + 32;
		i++;
	}
	return (str);
}