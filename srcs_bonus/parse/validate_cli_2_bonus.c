/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_cli_2_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iyahoui- <iyahoui-@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 13:01:03 by iyahoui-          #+#    #+#             */
/*   Updated: 2022/07/01 13:21:38 by iyahoui-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

int	validate_parenthesis_contents(char *open_par_pos, int len)
{
	char	*parenthesis_contents;
	char	*ptr;
	int		ret;

	parenthesis_contents = ft_xalloc(len + 1);
	parenthesis_contents = ft_strncpy(parenthesis_contents, open_par_pos, len);
	if (!len)
		return (EMPTY_ARG);
	len--;
	while (ft_isspace(open_par_pos[len]) && len)
		len--;
	if (!len)
		return (EMPTY_ARG);
	ret = validate_input(parenthesis_contents);
	free(parenthesis_contents);
	parenthesis_contents = NULL;
	if (ret)
		return (ret);
	ptr = open_par_pos + len + 1;
	return (0);
}

int	validate_parenthesis(char **line, int *state, int nb_open_parentheses)
{
	char	*parenthesis;

	if (**line != '(')
		return (0);
	parenthesis = *line + 1;
	if (!(*state & EMPTY_ARG) && !(*state & REDIR))
		return (**line);
	while (**line)
	{
		if (**line == '(')
			nb_open_parentheses += 1;
		else if (**line == ')')
			nb_open_parentheses -= 1;
		if (nb_open_parentheses < 0)
			return (**line);
		skip_quotes(line);
		if (nb_open_parentheses == 0)
			break ;
		*line += 1;
	}
	if (nb_open_parentheses > 0)
		return (UNCLOSED_PARENTHESIS);
	*state = PARENTHESIS;
	return (validate_parenthesis_contents(parenthesis, *line - parenthesis));
}

int	validate_redir(char **line, int *state)
{
	if (!is_set(**line, "<>"))
		return (0);
	*line += 1;
	if (*(*line - 1) == **line)
		*line += 1;
	skip_whitespaces(line);
	while (**line)
	{
		if (skip_quotes(line))
			*state |= REDIR;
		else if (ft_isspace(**line) || is_set(**line, "()<>&|"))
			break ;
		else
			*state |= REDIR;
		*line += 1;
	}
	if (!(*state & REDIR))
		return (**line + (!**line) * '\n');
	return (0);
}

int	validate_job(char **line, int *state)
{
	if (is_set(**line, "|&"))
	{
		if (((*state & EMPTY_ARG) && !(*state & REDIR)))
			return (**line);
		*state = EMPTY_ARG;
		if (*(*line + 1) == **line)
			*line += 1;
	}
	return (0);
}
