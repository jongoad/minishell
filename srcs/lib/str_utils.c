#include "minishell.h"

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

	/* Added by Ismael: sorry, but it makes it easier for me below */
	if (!s)
		return (0);
	i = 0;
	while (s[i] != '\0')
		i++;
	return (i);
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

/* Find if a character exists in a string */
int	is_set(char const c, char const *set)
{
	int	i;

	if (!set || !c)
		return (0);
	i = 0;
	while (set[i])
	{
		if (set[i] == c)
			return (1);
		i++;
	}
	return (0);
}

/* Find the position of the first instance of a character in a string. If not found returns -1 */
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

char	*ft_strncpy(char *dest, char *src, size_t n)
{
	size_t	i;

	i = 0;
	if (!src)
		return (dest);
	while (src[i] != 0 && i < n)
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = 0;
	return (dest);
}

char	*get_first_token(char *string, char delim)
{
	size_t	str_len;
	char	*first_token;
	size_t	first_token_len;
	size_t	i;

	if (!string)
		return (NULL);
	str_len = ft_strlen(string);
	first_token_len = 0;
	if (!string)
		return (NULL);
	while (string[first_token_len] != delim && first_token_len < str_len)
		first_token_len++;
	first_token = ft_xalloc(first_token_len + 1);
	i = 0;
	while (i < first_token_len)
	{
		first_token[i] = string[i];
		i++;
	}
	first_token[i] = 0;
	return (first_token);
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

char	*ft_strjoin_free(char *s1, char const *s2)
{
	size_t	tot_len;
	char	*s_joined;
	int		i;

	tot_len = ft_strlen(s1) + ft_strlen(s2) + 1;
	if (!s1 && !s2)
		return (NULL);
	s_joined = ft_xalloc(tot_len);
	i = 0;
	if (s1)
		while (s1[i])
			*s_joined++ = s1[i++];
	free (s1);
	s1 = NULL;
	i = 0;
	if (s2)
		while (s2[i])
			*s_joined++ = s2[i++];
	*s_joined++ = 0;
	return (s_joined - tot_len);
}

int	ft_isalnum(int c)
{
	if ((c >= '0' && c <= '9')
		|| (c >= 'A' && c <= 'Z')
		|| (c >= 'a' && c <= 'z'))
		return (1);
	else
		return (0);
}

/* Goes with ft_itoa */
static int	power_calc(int base, int power)
{
	if (power < 0)
		return (0);
	else if (power == 0)
		return (1);
	return (base * power_calc(base, power - 1));
}

char	*ft_itoa(int n)
{
	char	*n_to_a;
	long	n_to_long;
	int		magnitude;
	int		len;

	n_to_long = n;
	len = 0;
	while (n && ++len)
		n /= 10;
	n_to_a = ft_xalloc(len + 2);
	if (n_to_long < 0)
	{
		n_to_a[n++] = '-';
		n_to_long *= -1;
	}
	while (len--)
	{
		magnitude = power_calc(10, len);
		n_to_a[n++] = (n_to_long / magnitude) + '0';
		n_to_long %= magnitude;
	}
	n_to_a[n] = 0;
	return (n_to_a);
}

/* Convert a string to an int */
int	ft_atoi(const char *str)
{
	long long	num;
	long long	sign;

	num = 0;
	sign = 1;
	while ((*str >= 9 && *str <= 13) || *str == 32)
		str++;
	if (*str == '+' || *str == '-')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		num = (num * 10) + (*str - '0');
		str++;
		if ((num * sign) > INT_MAX)
			return (-1);
		if ((num * sign) < INT_MIN)
			return (0);
	}
	return (num * sign);
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
