/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iyahoui- <iyahoui-@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/09 14:34:09 by jgoad             #+#    #+#             */
/*   Updated: 2022/06/16 18:00:57 by iyahoui-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

/* Write a character to specified file descriptor */
int	putchar_fd(char c, int fd)
{
	write(fd, &c, sizeof(c));
	return (sizeof(c));
}

/* Write a string to specified file descriptor */
int	putstr_fd(char *str, int fd)
{
	int	count;

	count = 0;
	while (*str)
	{
		count += putchar_fd(*str, fd);
		str++;
	}
	return (count);
}

/* Write a string to specified file descriptor */
int	putendl_fd(char *str, int fd)
{
	int	count;

	count = 0;
	while (*str)
	{
		count += putchar_fd(*str, fd);
		str++;
	}
	count += putstr_fd("\n", fd);
	return (count);
}
