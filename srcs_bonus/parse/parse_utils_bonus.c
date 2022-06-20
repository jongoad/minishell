/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iyahoui- <iyahoui-@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/13 16:59:24 by jgoad             #+#    #+#             */
/*   Updated: 2022/06/20 17:17:15 by iyahoui-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

void	skip_whitespaces(char **line)
{
	if (!line || !*line)
		return ;
	while (**line && is_set(**line, WHITESPACES))
		*line += 1;
}

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

int	get_parenthesis_len(char *line)
{
	int	len;
	int	open_parentheses;

	len = 1;
	open_parentheses = 1;
	line += 1;
	while (*line)
	{
		skip_quotes(&line);
		if (*line == '(')
			open_parentheses += 1;
		else if (*line == ')')
			open_parentheses -= 1;
		if (open_parentheses == 0)
			break ;
		line += 1;
		len++;
	}
	return (len);
}
