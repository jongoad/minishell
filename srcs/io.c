#include "minishell.h"

/* Check input and output files and set up file descriptors */
int init_io(t_shell *sh, t_cmd *cmd)
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
	int i;

	i = -1;
	while (++i < cmd->nb_ins)
	{
		cmd->errnum = access(cmd->ins[i]->infile, F_OK);
		if (!cmd->errnum)
			cmd->errnum = access(cmd->ins[i]->infile, R_OK);
		if (cmd->errnum)
		{
			put_err_msg(sh->sh_name, cmd->ins[i]->infile, NULL, strerror(cmd->errnum));
			close_files(cmd); //Maybe move
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
			cmd->outs[i]->fd = open(cmd->outs[i]->outfile, O_CREAT | O_RDWR | O_APPEND, 0000644);
		else
			cmd->outs[i]->fd = open(cmd->outs[i]->outfile, O_CREAT | O_RDWR | O_TRUNC, 0000644);
		if (cmd->outs[i]->fd < 0)
		{
			cmd->errnum = errno;
			put_err_msg(sh->sh_name, cmd->ins[i]->infile, NULL, strerror(cmd->errnum));
			close_files(cmd); //Maybe move
			return (cmd->errnum);
		}
		if (i)
			close (cmd->outs[i - 1]->fd);
	}
	if (i)
		cmd->fd_out = cmd->outs[i - 1]->fd;
	else
		cmd->fd_out = 1;
	return (cmd->errnum);
}
