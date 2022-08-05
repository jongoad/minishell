/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_cli_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iyahoui- <iyahoui-@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 13:01:03 by iyahoui-          #+#    #+#             */
/*   Updated: 2022/08/05 17:40:13 by iyahoui-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

int	validate_char(char **line, int *state)
{
	int	ret;

	skip_whitespaces(line);
	if (**line == ')')
		return (**line);
	ret = validate_parenthesis(line, state, 0);
	if (ret)
		return (ret);
	if ((**line == '&' && *(*line + 1) != '&'))
		return (**line);
	ret = validate_job(line, state);
	if (ret)
		return (ret);
	ret = validate_redir(line, state);
	if (ret)
		return (ret);
	if ((*state & PARENTHESIS) && **line
		&& !ft_isspace(**line) && !is_set(**line, "()<>&|"))
		return (**line);
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
	bool	is_empty_line;

	line_ptr = line;
	skip_whitespaces(&line_ptr);
	if (!*line_ptr)
		return (0);
	is_empty_line = true;
	state = EMPTY_ARG;
	while (*line_ptr)
	{
		ret = validate_char(&line_ptr, &state);
		if (state)
			is_empty_line = false;
		if (ret)
			return (ret);
		if (*line_ptr)
			line_ptr++;
	}
	if (!is_empty_line && (state & EMPTY_ARG) && !(state & REDIR))
		return ('\n');
	return (0);
}
