/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iyahoui- <iyahoui-@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/13 15:56:34 by jgoad             #+#    #+#             */
/*   Updated: 2022/06/13 16:45:44 by iyahoui-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Check if history entry is not just empty line */
bool	is_valid_hist_entry(char *entry)
{
	int	ret;
	int	i;

	i = 0;
	ret = false;
	while (entry[i])
	{
		if (entry[i] < 9 || (entry[i] > 13 && entry[i] != 32))
			ret = true;
		i++;
	}
	*(entry + i) = '\0';
	return (ret);
}

/* Init history */
void	init_history(t_shell *sh)
{
	char	*curr_line;
	char	*hist_path;

	hist_path = ft_strjoin(sh->ms_path, HIST_LOG);
	sh->history_fd = open(hist_path, O_CREAT | O_RDONLY, 0644);
	if (sh->history_fd < 0)
		return ;
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
	sh->history_fd = open(hist_path, O_WRONLY | O_APPEND, 0644);
	free(hist_path);
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
	sh->line = readline (sh->prompt);
	if (sh->line && *sh->line)
	{
		if (is_valid_hist_entry(sh->line))
			add_history(sh->line);
		putendl_fd(sh->line, sh->history_fd);
	}
	return (sh->line);
}
