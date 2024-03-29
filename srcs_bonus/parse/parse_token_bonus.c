/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_token_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iyahoui- <iyahoui-@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/13 16:58:55 by jgoad             #+#    #+#             */
/*   Updated: 2022/08/06 18:32:13 by iyahoui-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

void	parse_dquotes(t_arglst **lst, char **line)
{
	int		tok_len;

	tok_len = get_tok_len(*line + 1, "\"");
	if (!(*line)[tok_len + 1])
		return (add_token(lst, line, CL_DQU_LIM, false));
	*line += 1;
	while (**line)
	{
		tok_len = get_tok_len(*line, "\"$");
		if ((*line)[tok_len] == '\"')
		{
			add_token(lst, line, "\"", false);
			*line += 1;
			return ;
		}
		else if (**line == '$')
			parse_env_var(lst, line);
		else
			add_token(lst, line, CL_DQU_NOSPEC, false);
	}
}

void	parse_squotes(t_arglst **lst, char **line)
{
	int		tok_len;

	tok_len = get_tok_len(*line + 1, "\'");
	if (!(*line)[tok_len + 1])
		return (add_token(lst, line, CL_SQU_LIM, false));
	*line += 1;
	add_token(lst, line, "\'", false);
	*line += 1;
}

void	parse_env_var(t_arglst **lst, char **line)
{
	*line += 1;
	if (**line && (ft_isalnum(**line)
			|| (**line == '?' && (is_set(*(*line + 1), CL_TOK_LIM) || !*(*line + 1)))))
	{
		if (**line == '?')
			add_token_by_len(lst, line, 1, true);
		else
			add_token_by_set(lst, line, ENV_VAR_CHARS, true);
	}
	else
	{
		*line -= 1;
		add_token(lst, line, CL_VAR_LIM, false);
	}
}

void	parse_wildcard(t_arglst **lst, char **line)
{
	add_token_by_len(lst, line, 1, true);
	while (**line && **line == '*')
		*line += 1;
}

void	parse_parenthesis(t_arglst **lst, char **line)
{
	char	*arg;
	int		parenthesis_len;

	parenthesis_len = get_parenthesis_len(*line);
	arg = ft_xalloc(parenthesis_len + 2);
	arg = ft_strncpy(arg, *line, parenthesis_len + 1);
	ms_lstadd(lst, ms_lstnew(arg, false));
	*line += parenthesis_len + 1;
	return ;
}
