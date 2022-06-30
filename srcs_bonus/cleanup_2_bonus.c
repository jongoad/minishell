/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_2_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iyahoui- <iyahoui-@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/09 15:16:30 by jgoad             #+#    #+#             */
/*   Updated: 2022/06/29 22:37:14 by iyahoui-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

/* Clean all memory for a single instance of command struct */
void	clean_single_cmd(t_cmd *cmd)
{
	int	i;

	close_files(cmd);
	free_array((void **)cmd->args);
	free(cmd->exe);
	cmd->exe = NULL;
	free(cmd->errname);
	cmd->errname = NULL;
	i = 0;
	while (cmd->args_lst && cmd->args_lst[i])
		ms_lstclear(&cmd->args_lst[i++]);
	clean_input(cmd);
	clean_output(cmd);
	free(cmd->ins);
	cmd->ins = NULL;
	free(cmd->outs);
	cmd->outs = NULL;
	free(cmd->args_lst);
	cmd->args_lst = NULL;
}

/* Clean input data */
void	clean_input(t_cmd *cmd)
{
	int	i;

	i = 0;
	while (i < cmd->nb_ins)
	{
		ms_lstclear(&cmd->ins[i]->in_lst);
		ms_lstclear(&cmd->ins[i]->delim_lst);
		if (cmd->ins[i]->delim)
			unlink(cmd->ins[i]->infile);
		free(cmd->ins[i]->infile);
		cmd->ins[i]->infile = NULL;
		free(cmd->ins[i]->delim);
		cmd->ins[i]->delim = NULL;
		free(cmd->ins[i]);
		cmd->ins[i] = NULL;
		i++;
	}	
}

/* Clean output data */
void	clean_output(t_cmd *cmd)
{
	int	i;

	i = 0;
	while (i < cmd->nb_outs)
	{
		ms_lstclear(&cmd->outs[i]->out_lst);
		cmd->outs[i]->out_lst = NULL;
		free(cmd->outs[i]->outfile);
		cmd->outs[i]->outfile = NULL;
		free(cmd->outs[i]);
		cmd->outs[i] = NULL;
		i++;
	}
}

/* Free command memory before returning to readline loop */
void	clean_cmds(t_shell *sh)
{
	int	i;

	i = 0;
	if (sh->pipes)
	{
		close_pipes(sh);
		free(sh->pipes);
		sh->pipes = NULL;
	}
	i = 0;
	while (sh->cmds && i < sh->nb_cmds)
	{
		if (sh->cmds[i])
		{
			close_files(sh->cmds[i]);
			clean_single_cmd(sh->cmds[i]);
			free(sh->cmds[i]);
			sh->cmds[i] = NULL;
		}
		i++;
	}
	free(sh->pids);
	sh->pids = NULL;
	free(sh->cmds);
	sh->cmds = NULL;
}

void	clean_jobs(t_shell *sh)
{
	t_ms_job	*ptr;
	t_ms_job	*prev;

	ptr = sh->jobs;
	while (ptr)
	{
		sh->cmds = ptr->cmds;
		sh->nb_cmds = ptr->nb_cmds;
		reset_shell(sh);
		prev = ptr;
		ptr = ptr->next;
		free(prev);
	}
	return ;
}
