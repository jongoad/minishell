#include "../includes/minishell.h"

/* Initialize pipes using number of commands and open them */
int	init_pipes(t_shell *sh)
{
	int i;

	i = 0;
	sh->nb_pipes = 2 * (sh->nb_cmds - 1);
	sh->pipes = (int *)malloc(sizeof(int) * sh->nb_pipes);
	if (!sh->pipes)
	{
		put_err_msg(NULL, NULL, NULL, ERR_PIPE);
		return (1);
	}
	while (i < sh->nb_cmds - 1)
	{
		if (pipe(sh->pipes + 2 * i) < 0)
		{
			put_err_msg(NULL, NULL, NULL, ERR_PIPE);
			return (1);
		}
		i++;
	}
	return (0);
}

/* Close all pipes */
void	close_pipes(t_shell *sh)
{
	int	i;

	i = 0;
	while (i < sh->nb_pipes)
	{
		close(sh->pipes[i]);
		i++;
	}
}

/* Update a single set of pipes */
void	update_pipes(int p1, int p2)
{
	dup2(p1, 0);
	dup2(p2, 1);
}

/* Manage pipes for each child process */
void	manage_pipes(t_shell *sh, t_cmd *cmd)
{
	int cmd_iter; /* need an iterator for command #  needs to be set elsewhere */
	/* Iterator must start at 1 or this one work!!!!! */


	if (cmd->ins->fd != -1)								/* If there is a redirect for input set first pipe to fd of redirect, else set first pipe to STDIN */
		update_pipes(cmd->ins->fd, sh->pipes[2 * sh->cmd_iter + 1]);
	if (cmd->outs->fd != -1) 							/*If there is a redirect for output set output pipe to fd, else set to STDOUT */
		update_pipes(sh->pipes[2 * cmd_iter - 2], cmd->outs->fd);
	else
		update_pipes(sh->pipes[2 * cmd_iter - 2], sh->pipes[2 * cmd_iter + 1]); /* If there is no redirect, connect pipes normally*/
	close_pipes(sh);
}

/* 
	- Redirection handled individually for each pipe 
	- If a pipe has an input redirect, anything on STDIN is discarded
	- File descriptors should be set to -1 if there is no redirection
*/




/* Build a path for a specific command */
static	char	*build_cmd_path(char **cmd_path, char *cmd)
{
	char	*tmp;
	char	*cmd_res;
	int		i;

	i = 0;
	while (cmd[i])
	{
		if (cmd[0] == '.' || cmd[i] == '/')
			return (ft_strdup(cmd));
		i++;
	}
	while (*cmd_path)
	{
		tmp = ft_strjoin(*cmd_path, "/");
		cmd_res = ft_strjoin(tmp, cmd);
		free (tmp);
		if (access(cmd_res, 0) == 0)
			return (cmd_res);
		free(cmd_res);
		cmd_path++;
	}
	return (NULL);
}