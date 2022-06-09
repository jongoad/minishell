/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoad <jgoad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/09 14:31:08 by jgoad             #+#    #+#             */
/*   Updated: 2022/06/09 14:31:12 by jgoad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

/* Read from fd into storage string */
int	read_chars(char **store, int fd)
{
	char	*buffer;
	int		status;

	while (!*store || !strlen_char(*store, '\n'))
	{
		buffer = malloc(BUFFER_SIZE + 1);
		status = read(fd, buffer, BUFFER_SIZE);
		if (status <= 0)
		{
			free(buffer);
			return (0);
		}
		buffer[status] = '\0';
		*store = join_free(*store, buffer);
		free(buffer);
	}
	return (strlen_char(*store, '\n'));
}

/* Get a single line to return from the currently stored string */
static void	get_line(char **store, char **line, int len)
{
	char	*new;
	int		i;

	i = 0;
	while (i < len)
	{
		(*line)[i] = (*store)[i];
		i++;
	}
	(*line)[i] = '\0';
	if (!strlen_char(*store, '\n'))
	{
		free(*store);
		*store = NULL;
	}
	else
	{
		new = ft_strdup_char(&(*store)[len]);
		free(*store);
		*store = new;
	}
}

/* Return a line from a source specified by fd */
char	*get_next_line(int fd)

{
	static char		*store[12288];
	char			*line;
	int				len;

	len = read_chars(&store[fd], fd);
	if (!len)
		len = strlen_char(store[fd], '\0');
	if (!len)
	{
		free (store[fd]);
		store[fd] = NULL;
		return (NULL);
	}
	line = malloc(len + 1);
	get_line(&store[fd], &line, len);
	return (line);
}
