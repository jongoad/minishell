/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_token.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iyahoui- <iyahoui-@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/13 16:54:32 by jgoad             #+#    #+#             */
/*   Updated: 2022/06/13 17:16:17 by iyahoui-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_token(t_arglst **lst, char **line, char *delim, bool is_env_var)
{
	char	*token;
	int		tok_len;

	tok_len = get_tok_len(*line, delim);
	token = ft_xalloc((tok_len + 1) * sizeof(char));
	token = ft_strncpy(token, *line, tok_len);
	ms_lstadd(lst, ms_lstnew(token, is_env_var));
	*line += tok_len;
	return ;
}

void	add_token_by_set(
	t_arglst **lst, char **line, char *set, bool is_env_var)
{
	char	*token;
	int		tok_len;

	tok_len = get_tok_len_set(*line, set);
	token = ft_xalloc((tok_len + 1) * sizeof(char));
	token = ft_strncpy(token, *line, tok_len);
	ms_lstadd(lst, ms_lstnew(token, is_env_var));
	*line += tok_len;
	return ;
}

void	add_token_by_len(
	t_arglst **lst, char **line, int tok_len, bool is_env_var)
{
	char	*token;

	token = ft_xalloc((tok_len + 1) * sizeof(char));
	token = ft_strncpy(token, *line, tok_len);
	ms_lstadd(lst, ms_lstnew(token, is_env_var));
	*line += tok_len;
	return ;
}
