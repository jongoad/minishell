#include "minishell.h"

/* Check return values from children and control process waiting */
static int	handle_error_return(t_shell *sh)
{
	int	stat_loc;
	int	ret;

	ret = wait(&stat_loc);
	if (ret <= 0)
		return (0);
	// printf("ret = %d\n", ret);
	// printf("WIFEXITED(sh->ret_val) = %d\n", WIFEXITED(stat_loc));
	// printf("WEXITSTATUS(sh->ret_val) = %d\n", WEXITSTATUS(stat_loc));
	// printf("WTERMSIG(sh->ret_val) = %d\n", WTERMSIG(stat_loc));
	// printf("sh->pids[sh->nb_cmds - 1] = %d\n", sh->pids[sh->nb_cmds - 1]);
	if (ret == sh->pids[sh->nb_cmds - 1])
	{
		if (WIFEXITED(stat_loc))
			sh->ret_val = WEXITSTATUS(stat_loc);
		else if (WTERMSIG(stat_loc) )
			sh->ret_val = 128 + WTERMSIG(stat_loc);
		// printf("ret_val = %d\n", sh->ret_val);
	}
	// printf("exited with ret_val = %d\n", sh->ret_val);
	return (ret);
}

/* Control function for executing commands */
void	execute(t_shell *sh)
{
	int	i;

	i = 0;
	sh->cmd_iter = 0;															/* Iterator for pipe control */
	sh->pids = ft_xalloc(sizeof(pid_t) * sh->nb_cmds);
	if (init_pipes(sh))
		return ;
	while (i < sh->nb_cmds)
	{
		check_builtins(sh, sh->cmds[i]);										/* Check if command is builtin, and if so get the function index */
		if (BONUS)
			check_wildcard(sh->cmds[i]);
		if (sh->nb_cmds == 1 && sh->cmds[i]->builtin >= 0)						/* If there is only one command and it is a builtin, run it without forking */
			sh->ret_val = run_builtin_parent(sh, sh->cmds[i]);
		else
			run_cmd(sh, sh->cmds[i], i);										/* Run command(s) in forked processes */
		close_files(sh->cmds[i]);
		i++;
		sh->cmd_iter++;
	}
	close_pipes(sh);
	// i = -1;
	// while (++i < sh->nb_cmds)
	// 	printf("sh->pids[%d] = %d\n", i, sh->pids[i]);
	if (sh->nb_cmds > 1 || (sh->nb_cmds == 1 && sh->cmds[0]->builtin < 0))		/* Wait unless there was only one command and it was a builtin */
		while (handle_error_return(sh))
			;
}

/* Fork process and run a command */
void	run_cmd(t_shell *sh, t_cmd *cmd, int i)
{
	int		ret;

	sh->pids[i] = fork();
	if (sh->pids[i] == 0)
	{
		if (!init_io(sh, cmd) && cmd->exe)										/* Check IO and presence of a command */
		{
			manage_pipes(sh, cmd);
			if (cmd->builtin < 0)												/* If system command run with execve */
				run_cmd_external(sh, cmd);
			else																/* If built in command run in current process */
			{
				cmd->errnum = sh->builtins.f[cmd->builtin](sh, cmd);
				if (cmd->errnum)
					put_err_msg(sh->sh_name, cmd->exe, NULL, ERR_PIPE);			/* Check getting correct error message here */
			}
		}
		ret = cmd->errnum;
		close_files(cmd);
		cleanup(sh);
		exit(ret);
	}
}

/* Run an external command using execve */
void	run_cmd_external(t_shell *sh, t_cmd *cmd)
{
	char	*exe;
	
	exe = build_cmd_path(sh->env.path, cmd->exe);
	if (exe)
		cmd->errnum = execve(exe, cmd->args, sh->env.envp);
	cmd->errnum = E_CMDNOTFOUND;
	put_err_msg(sh->sh_name, cmd->exe, NULL, ERR_CMD);
}

/* Run builtin command without forking */
int	run_builtin_parent(t_shell *sh, t_cmd *cmd)
{
	if (init_io(sh, cmd))
		return (cmd->errnum);
	else
		sh->builtins.f[cmd->builtin](sh, cmd);
	return (cmd->errnum);
}
