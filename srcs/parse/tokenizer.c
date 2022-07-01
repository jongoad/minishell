/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iyahoui- <iyahoui-@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/13 16:59:59 by jgoad             #+#    #+#             */
/*   Updated: 2022/07/01 17:54:02 by iyahoui-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	else
		add_token(lst, line, CL_TOK_LIM, false);
	if (**line && !ft_isspace(**line) && !is_set(**line, "|<>"))
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
	else
		add_token(lst, line, CL_TOK_LIM, false);
	ms_lstlast(*lst)->is_joined = true;
	if (**line && !ft_isspace(**line) && !is_set(**line, "|<>"))
		return (add_to_token(lst, line));
	return ;
}
