#include "minishell_bonus.h"

void	skip_quotes(char **line)
{
	char	*ptr;
	char	c;

	c = **line;
	if (is_set(c, "\"\'"))
	{
		ptr = ft_strchr(*line + 1, c);
		if (ptr)
			*line = ptr;
	}
}

int	validate_parenthesis_contents(char *open_par_pos, int len)
{
	char	*parenthesis_contents;
	char	*ptr;
	int		ret;

	parenthesis_contents = ft_xalloc(len + 1);
	parenthesis_contents = ft_strncpy(parenthesis_contents, open_par_pos, len);
	ret = validate_input(parenthesis_contents);
	printf("%s:%d : parenthesis contents: %s\nret = %d\n", __FUNCTION__, __LINE__, parenthesis_contents, ret);
	free(parenthesis_contents);
	parenthesis_contents = NULL;
	if (ret)
		return (ret);
	ptr = open_par_pos + len + 1;
	return (0);
}

int	validate_parenthesis(char **line, int *state)
{
	char	*ptr;
	int		open_parentheses;
	
	if (**line != '(')
		return (0);
	open_parentheses = 0;
	ptr = *line;
	if (!(*state & EMPTY_ARG))
	{
		printf("error : parenthesis reached while state = %d\n", *state);
		return (**line);
	}
	while (**line)
	{
		if (**line == '(')
			open_parentheses += 1;
		else if (**line == ')')
			open_parentheses -= 1;
		if (open_parentheses < 0)
			return (**line);
		else if (open_parentheses == 0)
			break ;
		skip_quotes(line);
		*line += 1;
	}
	if (open_parentheses > 0)
		return (UNCLOSED_PARENTHESIS);
	*state = PARENTHESIS;
	return (validate_parenthesis_contents(ptr, *line - ptr));
}

int	validate_char(char **line, int *state)
{
	int	ret;
	
	skip_whitespaces(line);
	printf("%s:%d : \'%c\', state = %d\n", __FUNCTION__, __LINE__, **line, *state);
	/* Check parenthesis validity should be a function call */
	if (**line == ')')
		return (**line);
	ret = validate_parenthesis(line, state);
	if (ret)
		return (ret);
	/* endcase */

	/* Check validity statement; should be a function call */
	if ((is_set(**line, "|&") && ((*state & EMPTY_ARG) || (*state & REDIR_CHAR)))
		|| (is_set(**line, "><") && (*state & REDIR_CHAR))
		|| (**line == '&' && *(*line + 1) != '&'))
		return (**line);
	/* endcase */

	/* Special char check */
	else if (is_set(**line, "><") || is_set(**line, "|&"))
	{
		if (((*state & EMPTY_ARG) || (*state & REDIR_CHAR)) && is_set(**line, "|&"))	// If new_token is hit, but last one is not valid
			return (**line);
		*state = EMPTY_ARG * is_set(**line, "|&");
		*state |= REDIR_CHAR * is_set(**line, "><");
		printf("%s:%d : char = %c, state = %d\n", __FUNCTION__, __LINE__, **line, *state);
		if (*(*line + 1) == **line)
			*line += 1;
	}
	/* endcase */
	
	/* Validity check needs to be a little more convoluted to encapsulate redirections but NOTHING else */
	else if (!ft_isspace(**line) && **line && !is_set(**line, "()<>&|"))
		*state = VALID;
	return (0);
}

int	validate_input(char *line)
{
	int	state;
	int	ret;

	/** Idea : parenthesis validation should be done in a roundabout way ;
			Each open parenthesis should be matched to a validate_parenthesis() call, 
				which checks that the amount of closed parentheses is at least equal to the amount of open ones.
			This way, validate_char() can simply return an error when it sees a ')',
				as validate_parenthesis() should have moved the line pointer past its own sets of brackets
	**/

	if (!line)
		return (-1);
	skip_whitespaces(&line);
	if (!*line)
		return (0);
	state = EMPTY_ARG;
	while (*line)
	{
		ret = validate_char(&line, &state);
		printf("ret = %d\n", ret);
		if (ret)
			return (ret);
		if (*line)
			line++;
	}
	if ((state & EMPTY_ARG) || (state & REDIR_CHAR))
		return ('\n');
	printf("line is valid\n");
	return (0);
}
