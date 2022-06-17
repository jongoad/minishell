/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_utils_2_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iyahoui- <iyahoui-@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/09 14:35:17 by jgoad             #+#    #+#             */
/*   Updated: 2022/06/16 23:27:50 by iyahoui-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

/* Calculate a power */
static int	power_calc(int base, int power)
{
	if (power < 0)
		return (0);
	else if (power == 0)
		return (1);
	return (base * power_calc(base, power - 1));
}

/* Convert and int to a string */
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
	if (!len)
		len = 1;
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

/* Check if an instance of a string exists within another string */
char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	unsigned int	i;
	unsigned int	count;

	count = 0;
	if (*needle == '\0')
		return ((char *)haystack);
	while (haystack && *haystack && count < len)
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

/* Join two strings and free the second input string */
char	*ft_strjoin_free_rev(char *s1, char *s2)
{
	size_t	tot_len;
	char	*s_joined;
	int		i;

	tot_len = ft_strlen(s1) + ft_strlen(s2) + 1;
	s_joined = malloc(tot_len);
	if (!s_joined)
		return (NULL);
	i = 0;
	if (s1)
	{
		while (s1[i])
			*s_joined++ = s1[i++];
	}
	i = 0;
	if (s2)
		while (s2[i])
			*s_joined++ = s2[i++];
	*s_joined++ = 0;
	free(s2);
	return (s_joined - tot_len);
}
