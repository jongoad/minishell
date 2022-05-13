#include "minishell.h"

void	skip_whitespaces(char **line)
{
	if (!line || !*line)
		return ;
	while (**line && is_set(**line, WHITESPACES))
		*line += 1;
}
