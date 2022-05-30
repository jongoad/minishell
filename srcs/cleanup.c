#include "minishell.h"

/* Free all memory before program exit */
void	cleanup(t_shell *sh)
{	
	clean_env(sh);
	clean_cmds(sh);
	if (sh->line)
		free(sh->line);
	if (sh->prompt)
		free(sh->prompt);
	if (sh->sh_name)
		free(sh->sh_name);
	if (sh)
		free(sh);
}

/* Free envp memory */
void clean_env(t_shell *sh)
{
	int	i;

	i = 0;
	if (sh->env.envp)
	{
		while (sh->env.envp[i])
		{
			if (sh->env.envp[i])
				free(sh->env.envp[i]);
			i++;
		}
		free(sh->env.envp);
	}
	i = 0;
	if (sh->env.path)
	{
		while (sh->env.path && sh->env.path[i])
		{
			if (sh->env.path[i])
				free(sh->env.path[i]);
			i++;
		}
		free(sh->env.path);
	}
}

void	clean_single_cmd(t_cmd *cmd)
{
	int	i;

	free_split(cmd->args);
	if (cmd->exe)
		free(cmd->exe);
	if (cmd->errname)
		free(cmd->errname);
	i = 0;
	while (i < cmd->nb_args)
		ms_lstclear(&cmd->args_lst[i++]);
	i = 0;
	while (cmd->nb_ins && i < cmd->nb_ins)
	{
		ms_lstclear(&cmd->ins[i]->in_lst);
		ms_lstclear(&cmd->ins[i]->delim_lst);
		if (cmd->ins[i]->delim)
			unlink(cmd->ins[i]->infile);
		if (cmd->ins[i]->infile)
			free(cmd->ins[i]->infile);
		if (cmd->ins[i]->delim)
			free(cmd->ins[i]->delim);
		i++;
	}
	i = 0;
	while (cmd->nb_outs && i < cmd->nb_outs)
	{
		ms_lstclear(&cmd->outs[i]->out_lst);
		if (cmd->outs[i]->outfile)
			free(cmd->outs[i]->outfile);
		i++;
	}
	if (cmd->ins)
		free(cmd->ins);
	if (cmd->outs)
		free(cmd->outs);
	if (cmd->args_lst)
		free(cmd->args_lst);
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
	}
	i = 0;
	while (sh->cmds && i < sh->nb_cmds)
	{
		if (sh->cmds[i])
		{
			clean_single_cmd(sh->cmds[i]);
			free(sh->cmds[i]);
		}
		i++;
	}
	if (sh->pids)
		free(sh->pids);
	if (sh->cmds)
		free(sh->cmds);
	sh->cmds = NULL;
}

/* Reset shell */
void reset_shell(t_shell *sh)
{
	if (sh->cmds)
		clean_cmds(sh);
	sh->nb_cmds = 0;
	sh->cmd_iter = 0;
	sh->nb_pipes = 0;
	sh->err_char = 0;
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
	while (cmd->ins && i < cmd->nb_ins)
	{
		if (cmd->ins[i]->fd != -1)
			close(cmd->ins[i]->fd);
		i++;
	}
	i = 0;
	while (cmd->outs && i < cmd->nb_outs)
	{
		if (cmd->outs[i]->fd != -1)
			close(cmd->outs[i]->fd);
		i++;
	}
}