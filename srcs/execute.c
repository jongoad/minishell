#include "minishell.h"

/* Control function for executing commands */
void	execute(t_shell *sh)
{
	int	i;

	i = 0;
	sh->cmd_iter = 0;															/* Iterator for pipe control */
	sh->pids = malloc(sizeof(pid_t) * sh->nb_cmds);
	if (init_pipes(sh))
		return;
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
	if (sh->nb_cmds > 1 || (sh->nb_cmds == 1 && sh->cmds[0]->builtin < 0))		/* Wait unless there was only one command and it was a builtin */
		while (wait(&sh->ret_val) > 0)
			sh->ret_val >>= 8 & 0xFF;											/* Update return value from each forked process */
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
		// FIXME: double error messaging for exit() with too many arguments
		if (cmd->errnum)
			put_err_msg(sh->sh_name, cmd->exe, NULL, NULL);						/* Check getting correct error message here FIX*/
	}
	return (cmd->errnum);
}

void	check_wildcards_debug(t_shell *sh)
{
	int	i;

	i = 0;
	while (i < sh->nb_cmds)
	{
		check_wildcard(sh->cmds[i]);
		i++;
	}
}

/* Wildcard integration function */
void	check_wildcard(t_cmd *cmd)
{
	char	**result;
	char	**output;
	int		i;

	result = NULL;
	output = NULL;
	i = 0;
	if (check_no_wildcard(cmd))
		return ;
	while (i < cmd->nb_args)
	{
		result = expand_wildcard(cmd->args[i]);
		if (!result)														/* If no result copy string to output array, adding back in the asterisk if it existed */
			output = add_str_array(output, replace_wildcard(cmd->args[i]));
		else																/* If there is a result copy result to output array */
			output = join_array_array(output, result);									
		i++;
	}
	free_array((void **)cmd->args);											/* Free args array and then assign new array to pointer */
	cmd->args = output;
	free(cmd->exe);															/* Free exe and set to first element of new args array */
	cmd->exe = ft_strdup(output[0]);
	cmd->nb_args = count_array((void **)output);							/* Update new args number */
}

/* Add wildcard character back to string */
char	*replace_wildcard(char *str)
{
	int	i;

	i = 0;
	while (str && str[i])
	{
		if (str[i] == WILDCARD)
			str[i] = '*';
		i++;
	}
	return (str);
}


/* Check if there are no wildcards in any of the args */
bool	check_no_wildcard(t_cmd *cmd)
{
	int	i;

	i = 0;
	while (i < cmd->nb_args)
	{
		if (is_set(WILDCARD, cmd->args[i]))
			return (false);
		i++;
	}
	return (true);
}

/* Join two string arrays together */
char	**join_array_array(char **arr1, char **arr2)
{
	char	**result;
	int		count;
	int		i;
	int		j;

	result = NULL;
	count = count_array(((void **)arr1)) + count_array((void**)arr2) + 1;
	result = ft_xalloc(sizeof(char *) * count);
	i = 0;
	while (arr1 && arr1[i])
	{
		result[i] = ft_strdup(arr1[i]);
		i++;
	}
	j = 0;
	while (arr2 && arr2[j])
	{
		result[i] = ft_strdup(arr2[j]);
		i++;
		j++;
	}
	free_array((void **)arr1);
	free_array((void **)arr2);
	return (result);
}
