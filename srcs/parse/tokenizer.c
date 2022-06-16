/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iyahoui- <iyahoui-@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/13 16:59:59 by jgoad             #+#    #+#             */
/*   Updated: 2022/06/16 14:50:30 by iyahoui-         ###   ########.fr       */
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
void	set_cl_tok(t_arglst **lst, char **line)
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
	if (**line && !ft_isspace(**line) && **line != '|')
		return (add_cl_tok(lst, line));
	return ;
}

// Adds tokens that belong together by setting the arglst->is_joined flag
void	add_cl_tok(t_arglst **lst, char **line)
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
	if (**line && !ft_isspace(**line) && **line != '|')
		return (add_cl_tok(lst, line));
	return ;
}

/**
 * @brief 	Set the cl token as a linked list of strings
 * 			Advances the `line' pointer to the end of the read token
 * 
 * @param lst	The address of the linked list to which the token will be added
 * @param line 	The pointer to the beginning of the token
 */
void	set_cl_tok_bonus(t_arglst **lst, char **line)
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
	else
		add_token(lst, line, CL_TOK_LIM_BONUS, false);
	if (**line && !ft_isspace(**line) && **line != '|')
		return (add_cl_tok_bonus(lst, line));
	return ;
}

// Adds tokens that belong together by setting the arglst->is_joined flag
void	add_cl_tok_bonus(t_arglst **lst, char **line)
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
	else
		add_token(lst, line, CL_TOK_LIM_BONUS, false);
	ms_lstlast(*lst)->is_joined = true;
	if (**line && !ft_isspace(**line) && **line != '|')
		return (add_cl_tok_bonus(lst, line));
	return ;
}
