/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoad <jgoad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/09 14:30:38 by jgoad             #+#    #+#             */
/*   Updated: 2022/06/09 14:31:04 by jgoad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Find the length of a string bassed on a specific string-end char */
int	strlen_char(char *str, char c)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i] != c && str[i])
		i++;
	if (!c)
		return (i);
	if (str[i] != c)
		return (0);
	else
		return (i + 1);
}

/* Return a malloced copy of a string based on a specific string-end char */
char	*ft_strdup_char(char *s1)
{
	unsigned int	i;
	char			*str;

	i = 0;
	str = malloc(strlen_char(s1, '\0') + 1);
	if (!str)
		return (NULL);
	while (s1[i])
	{
		str[i] = s1[i];
		i++;
	}
	str[i] = '\0';
	return (str);
}

/* Join two strings and free the first input string */
char	*join_free(char *store, char *buffer)
{
	char	*temp;
	int		i;

	i = 0;
	temp = malloc(strlen_char(store, '\0') + strlen_char(buffer, '\0') + 1);
	if (!temp)
		return (NULL);
	while (store && store[i])
	{
		temp[i] = store[i];
		i++;
	}
	while (*buffer)
	{
		temp[i] = *buffer++;
		i++;
	}
	temp[i] = '\0';
	free(store);
	return (temp);
}
