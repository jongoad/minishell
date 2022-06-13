#include "minishell.h"

void	skip_whitespaces(char **line)
{
	if (!line || !*line)
		return ;
	while (**line && is_set(**line, WHITESPACES))
		*line += 1;
}

/**
 * @brief Get the tok len object
 * 
 * @param token 
 * @param delimiters Set of chars that represent the end of token
 * @return int 		 tok_len until the first delimiter encountered
 * 					 -1 if an arg is NULL
 */
int	get_tok_len(char *token, char *delimiters)
{
	int	tok_len;

	if (!token)
		return (-1);
	tok_len = 0;
	while (token[tok_len] && !is_set(token[tok_len], delimiters))
	{
		tok_len++;
	}
	return (tok_len);
}

int	get_tok_len_set(char *token, char *set)
{
	int	tok_len;

	if (!token)
		return (-1);
	tok_len = 0;
	while (token[tok_len] && is_set(token[tok_len], set))
	{
		tok_len++;
	}
	return (tok_len);
}
