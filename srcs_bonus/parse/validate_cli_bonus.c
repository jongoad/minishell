#include "minishell_bonus.h"

int	check_parentheses(char *line, int state)
{
	if (!line || *line != ')')
		return (0);
	line++;
	skip_whitespaces(&line);
	while (*line)
	{
		if (!ft_isspace(*line) && !is_set(*line, "&|<>") && (state & PARENTHESIS))
			return (1);
		line++;
	}
	return (0);
}

int	validate_char(char **line, int *state, int *open_parentheses)
{
	skip_whitespaces(line);
	printf("char = %c(%d), state = %d\n", **line, **line, *state);
	if (**line == ')')
	{
		*open_parentheses -= 1;
		if (*open_parentheses < 0)
			return (**line);
		if (*open_parentheses == 0 && *state != EMPTY)
			*state |= PARENTHESIS;
	}
	if ((is_set(**line, "|&") && (*state == REDIR || *state == EMPTY))
		|| (is_set(**line, "><") && *state == REDIR)
		|| (**line == '&' && *(*line + 1) != '&'))
		return (**line);
	else if (**line == ')' && (*state & EMPTY))
		return (EMPTY);
	else if (is_set(**line, "><") || is_set(**line, "|&"))
	{
		if (*state & EMPTY)
			return (**line);
		*state = EMPTY * is_set(**line, "|&") + REDIR * is_set(**line, "><");
		printf("2: char = %c, state = %d\n", **line, *state);
		if (*(*line + 1) == **line)
			*line += 1;
	}
	else if (**line == '(')
	{
		if (*state != EMPTY)
			return (**line);
		*open_parentheses += 1;
		*state = EMPTY;
	}
	else if (check_parentheses(*line, *state))
		return (**line);
	else if (!ft_isspace(**line) && **line && !is_set(**line, "()<>&|"))
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
		printf("ret = %d\n", ret);
		if (ret)
			return (ret);
		if (*line)
			line++;
	}
	if ((state & EMPTY) || (state & REDIR))
		return ('\n');
	else if (open_parentheses)
		return (UNCLOSED_PARENTHESIS);
	printf("line is valid\n");
	return (0);
}
