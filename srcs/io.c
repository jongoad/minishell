/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iyahoui- <iyahoui-@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/13 16:35:51 by jgoad             #+#    #+#             */
/*   Updated: 2022/06/13 17:16:34 by iyahoui-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Check input and output files and set up file descriptors */
int	init_io(t_shell *sh, t_cmd *cmd)
{
	if (init_ins(sh, cmd))
		return (cmd->errnum);
	if (init_outs(sh, cmd))
		return (cmd->errnum);
	return (cmd->errnum);
}

/* Check input redirections and set up file descriptor */
int	init_ins(t_shell *sh, t_cmd *cmd)
{
	int	i;

	i = -1;
	while (++i < cmd->nb_ins)
	{
		cmd->errnum = access(cmd->ins[i]->infile, F_OK);
		if (!cmd->errnum)
			cmd->errnum = access(cmd->ins[i]->infile, R_OK);
		if (cmd->errnum)
		{
			if (errno == 2)
				put_err_msg(sh->sh_name, cmd->ins[i]->infile, NULL, ERR_FILE);
			else
				put_err_msg(sh->sh_name, cmd->ins[i]->infile, NULL, ERR_ACCESS);
			return (cmd->errnum);
		}
	}
	if (i)
	{
		cmd->ins[i - 1]->fd = open(cmd->ins[i - 1]->infile, O_RDONLY);
		cmd->fd_in = cmd->ins[i - 1]->fd;
	}
	else
		cmd->fd_in = 0;
	return (cmd->errnum);
}

/* Intialize outfiles */
int	init_outs(t_shell *sh, t_cmd *cmd)
{
	int	i;

	i = -1;
	while (++i < cmd->nb_outs)
	{
		if (cmd->outs[i]->append_mode)
			cmd->outs[i]->fd = open(cmd->outs[i]->outfile,
					O_CREAT | O_RDWR | O_APPEND, 0000644);
		else
			cmd->outs[i]->fd = open(cmd->outs[i]->outfile,
					O_CREAT | O_RDWR | O_TRUNC, 0000644);
		if (cmd->outs[i]->fd < 0)
		{
			cmd->errnum = errno;
			put_err_msg(sh->sh_name, cmd->ins[i]->infile,
				NULL, strerror(cmd->errnum));
			return (cmd->errnum);
		}
	}
	cmd->fd_out = 1;
	if (i)
		cmd->fd_out = cmd->outs[i - 1]->fd;
	return (cmd->errnum);
}
