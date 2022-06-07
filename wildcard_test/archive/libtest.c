
#include "library.h"
/* Count the number of elements in a 2d array that is null terminated */
int	count_array(void **array)
{
	int count;

	count = 0;
	while (array && array[count])
		count++;
	return (count);
}

/* Free a null terminated 2d array */
void	free_array(void **array)
{
	int	i;

	i = 0;
	if (array)
	{
		while(array[i])
		{
			free(array[i]);
			i++;
		}
		free(array);
	}
}

static unsigned int	string_count(char *s, char c)
{
	unsigned int	count;
	unsigned int	found;

	count = 0;
	found = 0;
	while (*s)
	{
		while (*s == c && *s)
		{
			found = 0;
			s++;
		}
		while (*s != c && *s)
		{
			if (found == 0)
				count++;
			found = 1;
			s++;
		}
	}
	return (count);
}

static unsigned int	char_count(char const *s, char c)
{
	unsigned int	count;

	count = 0;
	while ((*s != c) && *s)
	{
		count++;
		s++;
	}
	return (count);
}

static char	*get_string(char const *s, char c)
{
	char			*str;
	unsigned int	i;

	i = 0;
	str = malloc(sizeof(char) * char_count(s, c) + 1);
	if (!str)
		return (NULL);
	while ((*s != c) && *s)
	{
		str[i] = *s;
		i++;
		s++;
	}
	str[i] = '\0';
	return (str);
}

static void	free_array_split(char **array, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		free(array[i]);
		i++;
	}
	free(array);
}

char	**ft_split(char const *s, char c)
{
	char	**array;
	int		count;
	int		i;

	i = -1;
	if (!s)
		return (NULL);
	count = string_count((char *)s, c);
	array = (char **)malloc(sizeof(char *) * (count + 1));
	if (!array)
		return (NULL);
	array[count] = NULL;
	while (++i < count)
	{
		while (*s == c)
			s++;
		array[i] = get_string((char *)s, c);
		if (!array[i])
		{
			free_array_split(array, i);
			return (NULL);
		}
		s += ft_strlen(array[i]);
	}
	return (array);
}

/* Return a malloced copy of a strin */
char	*ft_strdup(const char *s1)
{
	unsigned int	i;
	char			*str;

	if (!s1)
		return (0);
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

	if (!s1 || !s2)
		return (0);
	i = 0;
	while ((s1[i] || s2[i]) && i < n)
	{
		if ((unsigned char)s1[i] != (unsigned char)s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return (0);
}

/* Return the length of a string */
size_t	ft_strlen(const char *s)
{
	unsigned int	i;

	/* Added by Ismael: sorry, but it makes it easier for me below */
	if (!s)
		return (0);
	i = 0;
	while (s[i] != '\0')
		i++;
	return (i);
}

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	unsigned int	i;
	unsigned int	count;

	count = 0;
	if (*needle == '\0')
		return ((char *)haystack);
	while (*haystack && count < len)
	{
		count = 0;
		i = 0;
		while (*(haystack + i) == needle[i] && count < len)
		{	
			if (needle[i + 1] == '\0')
				return ((char *)haystack);
			i++;
			count++;
		}
		haystack++;
		len --;
	}
	return (NULL);
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

char	*get_last_token(char *string, char delim)
{
	char	*last_token;
	size_t	str_len;
	size_t	i;
	size_t	j;

	if (!string)
		return (NULL);
	str_len = ft_strlen(string);
	i = str_len;
	if (!string)
		return (NULL);
	while (string[i] != delim && i >= 0)
		i--;
	last_token = ft_xalloc(str_len - i + 1);
	j = 0;
	while (i++ < str_len)
	{
		last_token[j] = string[i];
		j++;
	}
	last_token[j] = 0;
	return (last_token);
}

void	*ft_xalloc(size_t size)
{
	void	*mem_space;

	mem_space = malloc(size);
	if (!mem_space)
	{
		write(STDERR_FILENO, "Malloc Error\n", 13);
		exit(EXIT_FAILURE);
	}
	ft_bzero(mem_space, size);
	return (mem_space);
}

void	ft_bzero(void *s, size_t n)
{
	unsigned char	*ptr;

	ptr = s;
	while (n--)
		*ptr++ = 0;
}