/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iyahoui- <iyahoui-@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/13 16:52:01 by jgoad             #+#    #+#             */
/*   Updated: 2022/06/13 17:16:18 by iyahoui-         ###   ########.fr       */
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
