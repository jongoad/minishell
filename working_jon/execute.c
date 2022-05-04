/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoad <jgoad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/02 15:43:30 by jgoad             #+#    #+#             */
/*   Updated: 2022/05/04 13:43:08 by jgoad            ###   ########.fr       */
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
		//Need to check if built in command or not
		if (init_cmd(sh, cmd))
			msg_err_return();
		else if (cmd->builtin < 0)				/* If system command run with execve */
			execve(cmd->filepath, cmd->args, sh->env.envp);
		else									/* If built in command run in current process */
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
	
	int i;

	i = 0;
	/* Check infiles */
	while (cmd->in->infiles[i])
	{
		cmd->errnum = access(cmd->in->infiles[i], F_OK);
		if (!cmd->errnum)
			cmd->errnum = access(cmd->in->infiles[i], R_OK);
		if (cmd->errnum)
			return (cmd->errnum);
		i++;
	}
	/* Open last file and set fd */
	cmd->in->fd = open(cmd->in->infiles[i - 1], O_RDONLY);






	while (cmd->in->infiles[i])
	{
		if (access(cmd->in->infiles[i], F_OK))	/* Check F_OK */
		{
			cmd->errnum = errno;
			break;
		}
		else if (cmd)
	}
	if ()

	//Check outfiles
}