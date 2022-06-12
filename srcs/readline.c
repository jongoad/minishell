/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iyahoui- <iyahoui-@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/09 14:55:08 by jgoad             #+#    #+#             */
/*   Updated: 2022/06/12 13:11:26 by iyahoui-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_valid_hist_entry(char *entry)
{
	int	i;

	while (entry[i])
	{
		if (entry[i] < 9 || (entry[i] > 13 && entry[i] != 32))
			break ;
		i++;
	}
	if (entry[i])
		return (true);
	return (false);
}

/* Init history */
void	init_history(t_shell *sh)
{
	char	*curr_line;

	sh->history_fd = open(".minishell_history", O_CREAT | O_RDONLY, 0644);
	curr_line = get_next_line(sh->history_fd);
	while (curr_line)
	{
		if (is_valid_hist_entry(curr_line))
			add_history(curr_line);
		free(curr_line);
		curr_line = get_next_line(sh->history_fd);
	}
	free(curr_line);
	close(sh->history_fd);
	sh->history_fd = open(".minishell_history", O_WRONLY | O_APPEND, 0644);
	return ;
}

/* Read a string, and return a pointer to it.  Returns NULL on EOF. */
char	*rl_getline(t_shell *sh)
{
	if (sh->line)
	{
		free(sh->line);
		sh->line = (char *) NULL;
	}
	sh->line = readline (sh->prompt);		/* Get a line from the user. */
	if (sh->line && *sh->line)				/* If the line has any text in it, save it on the history. */
	{
		if (is_valid_hist_entry(curr_line))
			add_history(sh->line);
		putendl_fd(sh->line, sh->history_fd);
	}
	return (sh->line);
}
