/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iyahoui- <iyahoui-@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/09 15:16:30 by jgoad             #+#    #+#             */
/*   Updated: 2022/06/13 01:08:27 by iyahoui-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Free memory for command linked lists */
void	clean_linked_lists(t_shell *sh)
{
	int	i;

	i = 0;
	while (sh->cmds && i < sh->nb_cmds)
	{
		if (sh->cmds[i])
			clean_single_cmd_linked_lists(sh->cmds[i]);
		i++;
	}
}

/* Clean memory for a single array of command linked lists */
void	clean_single_cmd_linked_lists(t_cmd *cmd)
{
	int	i;

	i = 0;
	while (i < cmd->nb_args)
	{
		ms_lstclear(&cmd->args_lst[i]);
		cmd->args_lst[i] = NULL;
		i++;
	}
	i = 0;
	while (cmd->nb_ins && i < cmd->nb_ins)
	{
		ms_lstclear(&cmd->ins[i]->in_lst);
		cmd->ins[i]->in_lst = NULL;
		ms_lstclear(&cmd->ins[i]->delim_lst);
		cmd->ins[i]->delim_lst = NULL;
		i++;
	}
	i = 0;
	while (cmd->nb_outs && i < cmd->nb_outs)
	{
		ms_lstclear(&cmd->outs[i]->out_lst);
		cmd->outs[i]->out_lst = NULL;
		i++;
	}
}

/* Clean all memory for a single instance of command struct */
void	clean_single_cmd(t_cmd *cmd)
{
	int	i;

	free_array((void **)cmd->args);
	free(cmd->exe);
	cmd->exe = NULL;
	free(cmd->errname);
	cmd->errname = NULL;
	i = 0;
	while (cmd->args_lst[i])
		ms_lstclear(&cmd->args_lst[i++]);
	clean_io(cmd);
	free(cmd->ins);
	cmd->ins = NULL;
	free(cmd->outs);
	cmd->outs = NULL;
	free(cmd->args_lst);
	cmd->args_lst = NULL;
}

/* Clean memory and unlink files for input and output redirection */
void	clean_io(t_cmd *cmd)
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
		i++;
	}
	i = 0;
	while (i < cmd->nb_outs)
	{
		ms_lstclear(&cmd->outs[i]->out_lst);
		cmd->outs[i]->out_lst = NULL;
		free(cmd->outs[i]->outfile);
		cmd->outs[i]->outfile = NULL;
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
