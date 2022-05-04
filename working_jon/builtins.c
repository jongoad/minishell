#include "../includes/minishell.h"

/* Echo string */
void	builtin_echo(t_shell *sh, t_cmd *cmd)
{
	int	i;
	int	fd;

	i = 0;
	if (!ft_strcmp(cmd->args[0], "-n"))
	{
		echo_n(sh, cmd);
		return ;
	}
	// get the fd of the last outfile only
	fd = cmd->outs[cmd->nb_ins - 1].fd;
	while (cmd->args[i])
	{
		putstr_fd(cmd->args[i], fd);
		if (cmd->args[i + 1])
			putchar_fd(' ', fd);
		i++;
	}
	putchar_fd('\n', fd);
}

/* Echo string without newline */
void	builtin_echo_n(t_shell *sh, t_cmd *cmd)
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
}

/* Change directory */
void	builtin_cd(t_shell *sh, t_cmd *cmd)
{
	if (chdir(cmd->args[0]))
	{
		cmd->errnum = errno;
		cmd->errname = ft_strjoin("cd: ", cmd->args[0]);
	}
}
//Check use of perror here?

/* Print path to current directory */
void	builtin_pwd(t_shell *sh, t_cmd *cmd)
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
		putstr_fd(buf, cmd->io.out);
		putchar_fd('\n', cmd->io.out);
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
void	builtin_export(t_shell *sh, t_cmd *cmd)
{

}

/* Unset env var for current shell instance */
void	builtin_unset(t_shell *sh, t_cmd *cmd)
{

}

/* Print out env vars for current shell instance */
int	builtin_env(t_shell *sh, t_cmd *cmd)
{
	int	i;

	i = 0;
	if (cmd->errnum)
		return (msg_err_ret(cmd->errnum, cmd->errname));
	while (sh->env.envp[i])
	{
		pustr_fd(sh->env.envp[i], cmd->outs[cmd->nb_outs - 1].fd);
		putchar_fd('\n', cmd->outs[cmd->nb_outs - 1].fd);
		i++;
	}
	return (cmd->errnum);
}

/* Exit current shell instance */
void	builtin_exit(t_shell *sh, int ret)
{
	cleanup(sh);
	exit(ret);
}
