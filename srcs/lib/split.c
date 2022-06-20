/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iyahoui- <iyahoui-@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/09 14:33:42 by jgoad             #+#    #+#             */
/*   Updated: 2022/06/20 14:55:40 by iyahoui-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* Countnumber of strings to be split */
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

/* Count number of chars in a string to be split */
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

/* Get a single single to split */
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

/* Free an array of strings */
static void	free_array_split(char **array, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		free(array[i]);
		array[i] = NULL;
		i++;
	}
	free(array);
	array = NULL;
}

/* Main split function */
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
