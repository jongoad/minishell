#include "minishell.h"

/* Free all memory before program exit */
void	cleanup(t_shell *sh)
{	
	clean_env(sh);
	clean_cmds(sh);
	free(sh->line);
	sh->line = NULL;
	free(sh->prompt);
	sh->prompt = NULL;
	free(sh->sh_name);
	sh->sh_name = NULL;
	free(sh);
	sh = NULL;
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
			free(sh->env.envp[i]);
			sh->env.envp[i] = NULL;
			i++;
		}
		free(sh->env.envp);
	}
	i = 0;
	if (sh->env.path)
	{
		while (sh->env.path && sh->env.path[i])
		{
			free(sh->env.path[i]);
			sh->env.path[i] = NULL;
			i++;
		}
		free(sh->env.path);
	}
}

void	clean_single_cmd(t_cmd *cmd)
{
	int	i;

	free_array((void **)cmd->args);
	free(cmd->exe);
	cmd->exe = NULL;
	free(cmd->errname);
	cmd->errname = NULL;
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
		free(cmd->ins[i]->infile);
		cmd->ins[i]->infile = NULL;
		free(cmd->ins[i]->delim);
		cmd->ins[i]->delim = NULL;
		i++;
	}
	i = 0;
	while (cmd->nb_outs && i < cmd->nb_outs)
	{
		ms_lstclear(&cmd->outs[i]->out_lst);
		free(cmd->outs[i]->outfile);
		cmd->outs[i]->outfile = NULL;
		i++;
	}
	free(cmd->ins);
	cmd->ins = NULL;
	free(cmd->outs);
	cmd->outs = NULL;
	free(cmd->args_lst);
	cmd->args_lst = NULL;
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