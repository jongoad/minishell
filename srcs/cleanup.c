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
	free(sh->pwd);
	sh->pwd = NULL;
	free(sh->ms_path);
	sh->ms_path = NULL;
	free(sh);
	sh = NULL;
	clear_history();
}

/* Free envp memory */
void	clean_env(t_shell *sh)
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
	while (sh->env.path && sh->env.path[i])
	{
		free(sh->env.path[i]);
		sh->env.path[i] = NULL;
		i++;
	}
	free(sh->env.path);
	sh->env.path = NULL;
}

/* Reset shell */
void	reset_shell(t_shell *sh)
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
	close_files(cmd);
	cleanup(sh);
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
