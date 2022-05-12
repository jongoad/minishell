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

// int	init_pipes(t_shell *sh)
// {
// 	t_cmd	*cmd_i;
// 	size_t	i;
// 	int		pipe_fds[2];

// 	if (pipe(pipe_fds))
// 		return (E_PIPE);
// 	cmd_i = sh->cmds[0];
//	cmd_i->fd_in = STDIN_FILENO;
//	cmd_i->fd_out = pipe_fds[1];
// 	i = 1;
// 	while (i < sh->nb_cmds - 1)
// 	{
// 		cmd_i = first_cmd + i;
// 		cmd_i->in_fd = pipe_fds[0];
// 		if (pipe(pipe_fds))
// 			return (E_PIPE);
// 		if (i != nb_cmds - 1)
// 			cmd_i->out_fd = pipe_fds[1];
// 		i++;
// 	}
// 	cmd_i = first_cmd + i;
// 	cmd_i->in_fd = pipe_fds[0];
// 	cmd_i->out_fd = STDOUT_FILENO;
// 	return (0);
// }


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
	/* Handle input */
	if (cmd->fd_in > 0)
		dup2(cmd->fd_in, 0);
	else if (sh->cmd_iter == 0)
		dup2(STDIN_FILENO, 0);
	else
		dup2(sh->pipes[2 * sh->cmd_iter - 2], 0);

	/* Handle output */
	if (cmd->fd_out > 1)
		dup2(cmd->fd_out, 1);
	else if (sh->cmd_iter == sh->nb_cmds - 1)
		dup2(STDOUT_FILENO, 1);
	else
		dup2(sh->pipes[2 * sh->cmd_iter + 1], 1);

	close_pipes(sh);
}
