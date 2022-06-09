/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoad <jgoad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/09 14:55:08 by jgoad             #+#    #+#             */
/*   Updated: 2022/06/09 14:56:28 by jgoad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Init history */
void	init_history(t_shell *sh)
{
	char	*curr_line;

	sh->history_fd = open(".minishell_history", O_CREAT | O_RDONLY, 0644);
	curr_line = get_next_line(sh->history_fd);
	while (curr_line)
	{
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
		add_history(sh->line);
		putendl_fd(sh->line, sh->history_fd);
	}
	return (sh->line);
}
