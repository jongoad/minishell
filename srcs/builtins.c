#include "../includes/minishell.h"

/* Echo string */
void	echo(t_shell *sh, t_cmd *cmd)
{
	int	i;

	i = 0;
	if (cmd->errnum)
		return ;
	while (cmd->args[i])
	{
		putstr_fd(cmd->args[i], cmd->io.out);
		if (cmd->args[i + 1])
			putchar_fd(' ', cmd->io.out);
		i++;
	}
	putchar_fd('\n', cmd->io.out);
}

/* Echo string without newline */
void	echo_n(t_shell *sh, t_cmd *cmd)
{
	int	i;

	i = 0;
	while (cmd->args[i])
	{
		putstr_fd(cmd->args[i], cmd->io.out);
		if (cmd->args[i + 1])
			putchar_fd(' ', cmd->io.out);
		i++;
	}
}

/* Change directory */
void	cd(t_shell *sh, t_cmd *cmd)
{
	if (chdir(cmd->args[0]))
	{
		cmd->errnum = errno;
		cmd->errname = ft_strjoin("cd: ", cmd->args[0]);
	}
}
//Check use of perror here?

/* Print path to current directory */
void	pwd(t_shell *sh, t_cmd *cmd)
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
void	export(t_shell *sh, t_cmd *cmd)
{

}

/* Unset env var for current shell instance */
void	unset(t_shell *sh, t_cmd *cmd)
{

}

/* Print out env vars for current shell instance */
int	env(t_shell *sh, t_cmd *cmd)
{
	int	i;

	i = 0;
	if (cmd->errnum)
		return (msg_err_ret(cmd->errnum, cmd->errname));
	while (sh->env.envp[i])
	{
		pustr_fd(sh->env.envp[i], cmd->io.out);
		putchar_fd('\n', cmd->io.out);
		i++;
	}
	return (cmd->errnum);
}

/* Exit current shell instance */
void	sh_exit(t_shell *sh, int ret)
{
	cleanup(sh);
	exit(ret);
}
