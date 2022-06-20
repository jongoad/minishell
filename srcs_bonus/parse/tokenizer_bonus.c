/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iyahoui- <iyahoui-@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/13 16:59:59 by jgoad             #+#    #+#             */
/*   Updated: 2022/06/20 17:21:45 by iyahoui-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

/**
 * @brief 	Set the cl token as a linked list of strings
 * 			Advances the `line' pointer to the end of the read token
 * 
 * @param lst	The address of the linked list to which the token will be added
 * @param line 	The pointer to the beginning of the token
 */
void	tokenize(t_arglst **lst, char **line)
{
	if (!line || !*line)
		return ;
	if (**line == '\"')
		parse_dquotes(lst, line);
	else if (**line == '\'')
		parse_squotes(lst, line);
	else if (**line == '$')
		parse_env_var(lst, line);
	else if (**line == '*')
		parse_wildcard(lst, line);
	else if (**line == '(')
		parse_parenthesis(lst, line);
	else
		add_token(lst, line, CL_TOK_LIM_BONUS, false);
	if (**line && !ft_isspace(**line) && **line != '|')
		return (add_to_token(lst, line));
	return ;
}

// Adds tokens that belong together by setting the arglst->is_joined flag
void	add_to_token(t_arglst **lst, char **line)
{
	if (!line || !*line)
		return ;
	if (**line == '\"')
		parse_dquotes(lst, line);
	else if (**line == '\'')
		parse_squotes(lst, line);
	else if (**line == '$')
		parse_env_var(lst, line);
	else if (**line == '*')
		parse_wildcard(lst, line);
	else if (**line == '(')
		parse_parenthesis(lst, line);
	else
		add_token(lst, line, CL_TOK_LIM_BONUS, false);
	ms_lstlast(*lst)->is_joined = true;
	if (**line && !ft_isspace(**line) && **line != '|')
		return (add_to_token(lst, line));
	return ;
}
