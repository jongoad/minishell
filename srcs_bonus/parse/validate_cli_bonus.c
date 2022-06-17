#include "minishell_bonus.h"

int	validate_char(char **line, int *state, int *open_parentheses)
{
	printf("char = %c(%d), state = %d\n", **line, **line, *state);
	skip_whitespaces(line);
	if (**line == ')')
		*open_parentheses -= 1;
	if ((is_set(**line, "|&") && *state)
		|| (is_set(**line, "><") && *state == REDIR)
		|| (**line == ')' && *open_parentheses < 0)
		|| (**line == '&' && *(*line + 1) != '&'))
		return (**line);
	else if (**line == ')' && *state)
		return (EMPTY);
	else if (is_set(**line, "><") || is_set(**line, "|&"))
	{
		*state = EMPTY * is_set(**line, "|&") + REDIR * is_set(**line, "><");
		printf("2: char = %c, state = %d\n", **line, *state);
		if (*(*line + 1) == **line)
			*line += 1;
	}
	else if (**line == '(')
	{
		if (*state != is_empty)
			return (**line);
		*open_parentheses += 1;
		*state = EMPTY;
	}
	else if (!ft_isspace(**line) && **line)
		*state = VALID;
	return (0);
}

int	validate_input(char *line)
{
	int	state;
	int	ret;
	int	open_parentheses;

	if (!line)
		return (-1);
	skip_whitespaces(&line);
	if (!*line)
		return (0);
	state = EMPTY;
	open_parentheses = 0;
	while (*line)
	{
		ret = validate_char(&line, &state, &open_parentheses);
		if (ret)
			return (ret);
		if (*line)
			line++;
	}
	if (state)
		return ('\n');
	else if (open_parentheses)
		return ('(');
	return (0);
}
