/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoad <jgoad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/02 15:43:30 by jgoad             #+#    #+#             */
/*   Updated: 2022/05/03 17:32:41 by jgoad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	child_execute(t_shell *sh)
{
	int	i;

	i = 0;
	while (i < sh->nb_cmds)
	{
		run_cmd(sh, sh->cmds[i], i); //Need to handle forking etc, Should prob run builtins with execve for consistency
		i++;
	}
	i = 0;
	while (wait(&sh->ret_val) > 0)
		sh->ret_val >>= 8;
	clean_cmds(sh);	//Clear cmd data
	return ;
}

void	run_cmd(t_shell *sh, t_cmd *cmd, int i)
{
	sh->pids[i] = fork();
	if (sh->pids[i] == 0)
	{
		//Check filepath
		if (init_cmd(sh, cmd))
			msg_err_return();
		else
		{
			cd(sh, cmd);
			if (cmd->errnum)
				msg_err_ret(cmd->errnum, cmd->errname);
		}
		cleanup(sh);
		exit(cmd->errnum);
	}
	return ;
}

int init_cmd(t_shell *sh, t_cmd *cmd)
{
	//Check infiles
	int i;

	i = 0;
	while (cmd->io.infile[i])
	{
		if (access())
		{
			cmd->errnum = errno;
			break;
		}
	}
	if ()

	//Check outfiles
}