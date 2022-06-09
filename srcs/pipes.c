/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoad <jgoad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/09 14:56:39 by jgoad             #+#    #+#             */
/*   Updated: 2022/06/09 14:57:27 by jgoad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Initialize pipes using number of commands and open them */
int	init_pipes(t_shell *sh)
{
	int	i;

	i = 0;
	sh->nb_pipes = 2 * (sh->nb_cmds - 1);
	sh->pipes = (int *)malloc(sizeof(int) * sh->nb_pipes);
	if (!sh->pipes)
	{
		put_err_msg(NULL, NULL, NULL, ERR_PIPE);
		return (1);
	}
	while (i < sh->nb_cmds - 1)
	{
		if (pipe(sh->pipes + 2 * i) < 0)
		{
			put_err_msg(NULL, NULL, NULL, ERR_PIPE);
			return (1);
		}
		i++;
	}
	return (0);
}

/* Close all pipes */
void	close_pipes(t_shell *sh)
{
	int	i;

	i = 0;
	if (sh->pipes)
	{
		while (i < sh->nb_pipes)
		{
			close(sh->pipes[i]);
			i++;
		}
	}
}

/* Manage pipes for each child process */
void	manage_pipes(t_shell *sh, t_cmd *cmd)
{
	if (cmd->fd_in > 0)								/* Handle input */
		dup2(cmd->fd_in, 0);
	else if (sh->cmd_iter == 0)
		dup2(STDIN_FILENO, 0);
	else
		dup2(sh->pipes[2 * sh->cmd_iter - 2], 0);
	if (cmd->fd_out > 1)							/* Handle output */
		dup2(cmd->fd_out, 1);
	else if (sh->cmd_iter == sh->nb_cmds - 1)
		dup2(STDOUT_FILENO, 1);
	else
		dup2(sh->pipes[2 * sh->cmd_iter + 1], 1);
	close_pipes(sh);
}
