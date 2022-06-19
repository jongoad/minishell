#include "minishell_bonus.h"

#define IGREEN "\033[38;5;10m"
#define IRED "\033[38;5;9m"
#define RESET_COL "\033[0m"

int	skip_quotes(char **line)
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
	else
		return (0);
	return (1);
}

int	validate_parenthesis_contents(char *open_par_pos, int len)
{
	char	*parenthesis_contents;
	char	*ptr;
	int		ret;

	parenthesis_contents = ft_xalloc(len);
	parenthesis_contents = ft_strncpy(parenthesis_contents, open_par_pos, len - 1); // to discard trailing `)'
	printf("%s:%d : len = %d\n", __FUNCTION__, __LINE__, len);
	len--;
	while (ft_isspace(open_par_pos[len]) && len)
		len--;
	if (!len)
		return (EMPTY_ARG);
	ret = validate_input(parenthesis_contents);
	printf("%s:%d : parenthesis contents: %s\nret = %d\n", __FUNCTION__, __LINE__, parenthesis_contents, ret);
	free(parenthesis_contents);
	parenthesis_contents = NULL;
	if (ret)
		return ((ret == '\n') * ')' + ret);		// to account for nested parsing
	
	ptr = open_par_pos + len + 1;
	printf("%s:%d : ptr = %s\n", __FUNCTION__, __LINE__, ptr);
	return (0);
}

int	validate_parenthesis(char **line, int *state)
{
	char	*parenthesis;
	int		open_parentheses;
	
	if (**line != '(')
		return (0);
	open_parentheses = 0;
	parenthesis = *line + 1;	// to discard leading `('
	if (!(*state & EMPTY_ARG) && !(*state & REDIR))
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
		skip_quotes(line);
		*line += 1;
		if (open_parentheses == 0)
			break ;
	}
	if (open_parentheses > 0)
		return (UNCLOSED_PARENTHESIS);
	*state = PARENTHESIS;
	return (validate_parenthesis_contents(parenthesis, *line - parenthesis));
}

int	validate_redir(char **line, int *state)
{
	if (!is_set(**line, "<>"))
		return (0);
	printf("%s:%d : line = %s\n", __FUNCTION__, __LINE__, *line);
	*line += 1;
	if (*(*line - 1) == **line)
		*line += 1;
	skip_whitespaces(line);
	while (**line)
	{
		if (skip_quotes(line))
			*state |= REDIR;
		else if (ft_isspace(**line) || is_set(**line, "()<>&|"))
			break;
		else
			*state |= REDIR;
		*line += 1;
	}
	if (!(*state & REDIR))
		return (**line + (!**line) * '\n');
	return (0);
	
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
	if ((**line == '&' && *(*line + 1) != '&'))
		return (**line);
	/* endcase */

	/* Special char check */
	else if (is_set(**line, "|&"))
	{
		if (((*state & EMPTY_ARG) && !(*state & REDIR)))	// If new_token is hit, but last one is not valid
			return (**line);
		*state = EMPTY_ARG;
		printf("%s:%d : char = %c, state = %d\n", __FUNCTION__, __LINE__, **line, *state);
		if (*(*line + 1) == **line)
			*line += 1;
	}
	/* endcase */
	ret = validate_redir(line, state);
	if (ret)
		return (ret);
	/* Note: the current parsing seems to have issues with the states. 
		I need to review when to add a state and when to replace it. */

	if ((*state & PARENTHESIS) && !ft_isspace(**line) && **line && !is_set(**line, "()<>&|"))
		return (**line);

	/* Validity check needs to be a little more convoluted to encapsulate redirections but NOTHING else */
	else if (!ft_isspace(**line) && **line && !is_set(**line, "()<>&|"))
	{
		*state &= ~(EMPTY_ARG);
		*state = VALID;
	}
	return (0);
}

int	validate_input(char *line)
{
	char	*line_ptr;
	int		state;
	int		ret;

	if (!line)
		return (-1);
	line_ptr = line;
	skip_whitespaces(&line_ptr);
	if (!*line_ptr)
		return (0);
	state = EMPTY_ARG;
	while (*line_ptr)
	{
		ret = validate_char(&line_ptr, &state);
		printf("ret = %d\n", ret);
		if (ret)
			return (ret);
		if (*line_ptr)
			line_ptr++;
	}
	printf(IGREEN"-------------line is valid--------------"RESET_COL"\n");
	return (0);
}
