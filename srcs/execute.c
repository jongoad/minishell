#include "minishell.h"

/* Control function for executing commands */
void	execute(t_shell *sh)
{
	int	i;

	i = 0;
	sh->cmd_iter = 0;														/* Iterator for pipe control */
	sh->pids = malloc(sizeof(pid_t) * sh->nb_cmds);
	if (init_pipes(sh))
		return;
	while (i < sh->nb_cmds)
	{
		check_builtins(sh, sh->cmds[i]);									/* Check if command is builtin, and if so get the function index */
		if (sh->nb_cmds == 1 && sh->cmds[i]->builtin >= 0)					/* If there is only one command and it is a builtin, run it without forking */
			sh->ret_val = run_builtin_parent(sh, sh->cmds[i]);
		else
			run_cmd(sh, sh->cmds[i], i);									/* Run command(s) in forked processes */
		close_files(sh->cmds[i]);
		i++;
		sh->cmd_iter++;
	}
	close_pipes(sh);
	if (sh->nb_cmds > 1 || (sh->nb_cmds == 1 && sh->cmds[0]->builtin < 0))	/* Wait unless there was only one command and it was a builtin */
	{
		while (wait(&sh->ret_val) > 0)
			sh->ret_val >>= 8;												/* Update return value from each forked process */
	}
}
t_cmd * convert(t_cmd *sd)
{
	
}

/* Fork process and run a command */
void	run_cmd(t_shell *sh, t_cmd *cmd, int i)
{
	sh->pids[i] = fork();
	if (sh->pids[i] == 0)
	{
		if (!init_io(sh, cmd) && cmd->exe)										/* Check IO and presence of a command */
		{
			manage_pipes(sh, cmd);
			if (cmd->builtin < 0)												/* If system command run with execve */
			{
				cmd->errnum = execve(build_cmd_path(sh->env.path, cmd->exe), cmd->args, sh->env.envp);
				put_err_msg(sh->sh_name, cmd->exe, NULL, ERR_CMD);
			}
			else																/* If built in command run in current process */
			{
				cmd->errnum = sh->builtins.f[cmd->builtin](sh, cmd);
				if (cmd->errnum)
					put_err_msg(sh->sh_name, cmd->exe, NULL, ERR_PIPE);			/* Check getting correct error message here */
			}
		}
		close_files(cmd);
		cleanup(sh);
		exit(cmd->errnum);
	}
}

/* Run builtin command without forking */
int	run_builtin_parent(t_shell *sh, t_cmd *cmd)
{
	if (init_io(sh, cmd))
		return (cmd->errnum);
	else
	{
		sh->builtins.f[cmd->builtin](sh, cmd);
		if (cmd->errnum)
			put_err_msg(sh->sh_name, cmd->exe, NULL, NULL);		/* Check getting correct error message here */
	}
	return (cmd->errnum);
}
