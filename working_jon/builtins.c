#include "../includes/minishell.h"


//WHEN YOU LAUNCH BASH, THE PROMPT IS SET TO THE NAME YOU LAUNCHED IT WITH. aka bash will show bash-> BASH will show BASH->
//Echo needs to handle expansion, should probably just expand as each command is run


/* Echo string */
int	builtin_echo(t_shell *sh, t_cmd *cmd)
{
	int	i;
	int	fd;

	i = 0;
	if (!ft_strcmp(cmd->args[0], "-n"))
		return (echo_n(sh, cmd));
	fd = cmd->outs[cmd->nb_ins - 1].fd; /* Get the fd of the last outfile only */
	while (cmd->args[i])
	{
		putstr_fd(cmd->args[i], fd);
		if (cmd->args[i + 1])
			putchar_fd(' ', fd);
		i++;
	}
	putchar_fd('\n', fd);
	return (cmd->errnum);
}

/* Echo string without newline */
int	builtin_echo_n(t_shell *sh, t_cmd *cmd)
{
	int	i;
	int fd;

	i = 0;
	fd = cmd->outs[cmd->nb_outs - 1].fd;
	while (cmd->args[i])
	{
		putstr_fd(cmd->args[i], fd);
		if (cmd->args[i + 1])
			putchar_fd(' ', fd);
		i++;
	}
	return (cmd->errnum);
}

/* Change directory */
int	builtin_cd(t_shell *sh, t_cmd *cmd)
{
	if (chdir(cmd->args[0]))
	{
		cmd->errnum = errno;
		cmd->errname = ft_strjoin("cd: ", cmd->args[0]);
	}
}
//Check use of perror here?

/* Print path to current directory */
int	builtin_pwd(t_shell *sh, t_cmd *cmd)
{
	char *buf;

	if (cmd->errnum)
		return (msg_err_ret(cmd->errnum, cmd->errname));
	buf = (char *)malloc(sizeof(char) * 1025);
	buf = getcwd(buf, 1025);
	if (!buf)
		cmd->errnum = errno;
	else
	{
		putstr_fd(buf, cmd->outs[cmd->nb_outs - 1].fd);
		putchar_fd('\n', cmd->outs[cmd->nb_outs - 1].fd);
	}
	free(buf);
	if (cmd->errnum)
	{
		perror(strerror(cmd->errnum));
		return (cmd->errnum);
	}
	return (cmd->errnum);
}

/* Create a new env var for current shell instance */
int	builtin_export(t_shell *sh, t_cmd *cmd)
{
	int	i;
	
	i = 0;
	while (cmd->args[i])
	{
		
	}
}

/* Unset env var for current shell instance */
int	builtin_unset(t_shell *sh, t_cmd *cmd)
{

}

/* Print out env vars for current shell instance */
int	builtin_env(t_shell *sh, t_cmd *cmd)
{
	int	i;

	i = 0;
	if (cmd->errnum)
		exit (msg_err_ret(cmd->errnum, cmd->errname));
	while (sh->env.envp[i])
	{
		pustr_fd(sh->env.envp[i], cmd->outs[cmd->nb_outs - 1].fd);
		putchar_fd('\n', cmd->outs[cmd->nb_outs - 1].fd);
		i++;
	}
	exit(cmd->errnum);
}

/* Exit current shell instance */
int	builtin_exit(t_shell *sh, t_cmd *cmd)
{
	bool		is_valid;
	long long	ret;

	ret = 0;
	if (cmd->args[0])
	{
		ret = atoll(cmd->args[0], &is_valid);
		if (is_valid && cmd->args[1])
		{
			putstr_fd("exit\n", 2);
			putstr_fd(sh->prompt, 2);
			putstr_fd(": exit", 2);
			putstr_fd(ERR_EXIT_ARGS_NUM, 2);
			return (1);
		}
		else if (!is_valid)
		{
			putstr_fd(sh->prompt, 2);
			putstr_fd(": exit: ", 2);
			putstr_fd(cmd->args[0], 2);
			putstr_fd(ERR_EXIT_NON_NUMERIC, 2);
		}
		else
			putstr_fd("exit\n", 2);
	}
	cleanup(sh);						/* Cleanup ALL shell memory before exit */
	exit(ret);
	return (0); /* Return value just for consitency with other function prototypes */
}

/* Convert a value held in a string to an int and check if valid */
long long	atoll(const char *str, bool *is_valid)
{
	long long	num;
	long long	sign;
	long long	prev;

	num = 0;
	sign = 1;
	*is_valid = true;
	if (*str == '+' || *str == '-')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str)
	{
		if (*str < '0' || *str > '9')
			*is_valid = false;
		prev = num;
		num = (num * 10) + (*str - '0');
		str++;
		if (abs_val(num * sign) < prev)
			*is_valid = false;
	}
	return (num * sign);
}

/* Add a new environment variable to the envp array */
void	add_env_var(t_env *env, char *str)
{
	int	i;
	char **tmp;

	i = 0;
	while (env->envp[i])					/* Get count of envp array */
		i++;
	tmp = (char **)malloc(sizeof(char *) * i + 2);
	i = 0;
	while (env->envp[i])
	{
		tmp[i] = ft_strdup(env->envp[i]);
		i++;
	}
	tmp[i] = ft_strdup(str);
	tmp[i + 1] = NULL;
	i = -1;
	while (env->envp[++i])
		free(env->envp[i]);
	free(env->envp);
	env->envp = tmp;
}