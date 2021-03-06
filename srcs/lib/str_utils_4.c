/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_utils_4.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iyahoui- <iyahoui-@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/13 01:21:37 by iyahoui-          #+#    #+#             */
/*   Updated: 2022/06/13 01:21:39 by iyahoui-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_isspace(char c)
{
	if ((c > 8 && c < 14) || c == ' ')
		return (1);
	return (0);
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

/* Find the position of the first instance of a character
in a string. If not found returns -1 */
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
