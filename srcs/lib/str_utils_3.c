/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_utils_3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoad <jgoad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/09 14:35:28 by jgoad             #+#    #+#             */
/*   Updated: 2022/06/09 14:35:57 by jgoad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Copy n characters from src to dst */
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

/* Return a pointer to the position in a string following the
first instance of a delimeter character */
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

/* Return a malloced copy of the section of a string following
the last instance of a delimiter character */
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

/* Join two strings and free the first input string */
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

/* Check if a charcter is alphanumeric */
int	ft_isalnum(int c)
{
	if ((c >= '0' && c <= '9')
		|| (c >= 'A' && c <= 'Z')
		|| (c >= 'a' && c <= 'z'))
		return (1);
	else
		return (0);
}
