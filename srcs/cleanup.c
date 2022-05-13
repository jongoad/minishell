#include "minishell.h"

/* Free all memory before program exit */
void	cleanup(t_shell *sh)
{
	//Free cmd structs
	clean_cmds(sh);
	//Free shell struct
	free(sh->line);
	free(sh->pids);
	free(sh);
}

/* Free command memory before returning to readline loop */
void	clean_cmds(t_shell *sh)
{
	int	i;	

	close_pipes(sh);
	i = 0;
	while (i < sh->nb_cmds)
	{
		free_split(sh->cmds[i]->args);
		free(sh->cmds[i]->exe);
		free(sh->cmds[i]->errname);
		i++;
	}
	//Free cmd structs
	//Free pids
	//Free pipes
}

/* Clear local copy of data in fork before exiting */
void	clean_fork(t_shell *sh, t_cmd *cmd)
{
	close_files(cmd);							/* Close all open file descriptors */
	cleanup(sh);								/* Free all memory allocated in shell struct and its child structs */
}

/* Close all open file descriptors */
void	close_files(t_cmd *cmd)
{
	int i;

	i = 0;
	while (i < cmd->nb_ins)
	{
		if (cmd->ins[i]->fd != -1)
			close(cmd->ins[i]->fd);
		i++;
	}
	i = 0;
	while (i < cmd->nb_outs)
	{
		if (cmd->outs[i]->fd != -1)
			close(cmd->outs[i]->fd);
		i++;
	}
}