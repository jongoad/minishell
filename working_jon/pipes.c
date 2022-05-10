#include "../includes/minishell.h"

/* Create pipes based on the number of commands including here_doc */
void	open_pipes(t_shell *sh)
{
	int	i;

	i = 0;
	while (i < sh->nb_cmds - 1)
	{
		if (pipe(sh->pipes + 2 * i) < 0)
			//Free memory and return error
		i++;
	}
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

/* Initialize pipes using number of commands */
int	init_pipes(t_shell *sh)
{
	sh->nb_pipes = 2 * (sh->nb_cmds - 1);
	sh->pipes = (int *)malloc(sizeof(int) * sh->nb_pipes);
	if (!sh->pipes)
		error_msg(ERR_PIPE, 1);
	return (0);
}

/* Update a single set of pipes */
static	void	update_pipes(int p1, int p2)
{
	dup2(p1, 0);
	dup2(p2, 1);
}

/* Manage pipes for each child process */
static	void	manage_pipes(t_shell *sh, t_cmd *cmd)
{
	int cmd_iter; /* need an iterator for command #  needs to be set elsewhere */


	if (cmd->ins->fd != -1)								/* If there is a redirect for input set first pipe to fd of redirect, else set first pipe to STDIN */
		update_pipes(cmd->ins->fd, sh->pipes[2 * cmd_iter + 1]);
	if (cmd->outs->fd != -1) 							/*If there is a redirect for output set output pipe to fd, else set to STDOUT */
		update_pipes(sh->pipes[2 * cmd_iter - 2], ->fd_dst);
	else


	
		update_pipes(sh->pipes[2 * cmd_iter - 2], sh->pipes[2 * cmd_iter + 1]); /* If there is no redirect, connect pipes normally*/
		//This will not work unless the iter for the first command is 1 (2 * 0 - 2) == -2, cant access negative index
	close_pipes(d);
}

/* Redirection handled individually for each pipe 
	- If a pipe has an input redirect, anything on STDIN is discarded

	- File descriptors should be set to -1 if there is no redirection
*/


/* Run child based on current argument input */
void	run_child(t_data d, char **argv, char **envp)
{
	d.pid_child = fork();
	d.errnum = 0;
	if (!d.pid_child)
	{
		manage_pipes(&d);
		d.cmd_args = ft_split(argv[2 + d.here_doc + d.c_iter], ' ');
		d.cmd = build_cmd_path(d.cmd_path, d.cmd_args[0]);
		if (!d.cmd)
		{
			error_msg_child(d.cmd_args[0], ERR_CMD);
			d.errnum = 127;
		}
		else if (execve(d.cmd, d.cmd_args, envp) == -1)
		{
			error_msg_child(d.cmd, ERR_CMD);
			d.errnum = 127;
		}
		free_child(&d);
		if (d.errnum != 0)
			exit(d.errnum);
	}
}











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