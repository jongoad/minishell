#include "get_next_line.h"

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
