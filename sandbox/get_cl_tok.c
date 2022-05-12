#include "parse_test.h"

char	*get_cl_tok(char **line)
{
	char	*token;
	int		tok_len;

	if (!line || !*line)
		return (NULL);
	printf("line = %s\n", *line);
	token = *line;
	tok_len = 0;
	while (token[tok_len] && !is_set(token[tok_len], WHITESPACES))
	{
		printf("*line[%i] = %c\n", tok_len, token[tok_len]);
		tok_len++;
	}
	printf("after tok_len: %i\n", tok_len);
	if (!tok_len)
		return (NULL);
	token = ft_xalloc((tok_len + 1) * sizeof(char));
	tok_len = 0;
	while (**line && !is_set(**line, WHITESPACES))
	{
		token[tok_len] = **line;
		tok_len++;
		*line += 1;
	}
	token[tok_len] = 0;
	return (token);
}
