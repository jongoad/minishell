/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iyahoui- <iyahoui-@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/09 14:46:44 by jgoad             #+#    #+#             */
/*   Updated: 2022/06/12 13:52:57 by iyahoui-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Echo string */
int	builtin_echo(t_shell *sh, t_cmd *cmd)
{
	int	i;
	int	fd;

	i = 1;
	if (cmd->args[1] && !ft_strncmp(cmd->args[1], "-n", 3))
		return (builtin_echo_n(sh, cmd));
	fd = cmd->fd_out;
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
	int	fd;

	i = 2;
	fd = cmd->fd_out;
	while (cmd->args[i])
	{
		putstr_fd(cmd->args[i], fd);
		if (cmd->args[i + 1])
			putchar_fd(' ', fd);
		i++;
	}
	sh->ret_val = sh->ret_val;	//Janky fix, find a better use for sh in these functions
	return (cmd->errnum);
}

/* Change directory */
int	builtin_cd(t_shell *sh, t_cmd *cmd)
{
	char *buf;
	char *res;

	buf = (char *)malloc(sizeof(char) * 1025);
	buf = getcwd(buf, 1025);
	res = ft_strjoin("OLDPWD=", buf);
	free(buf);
	change_env_var(&sh->env, res);
	free(res);
	if (chdir(cmd->args[cmd->nb_args - 1]))
	{
		cmd->errnum = errno;
		cmd->errname = ft_strjoin("cd: ", cmd->args[cmd->nb_args - 1]);
	}
	else
	{
		buf = (char *)malloc(sizeof(char) * 1025);
		buf = getcwd(buf, 1025);
		res = ft_strjoin("PWD=", buf);
		free(buf);
		change_env_var(&sh->env, res);
		free(res);
	}
	return (cmd->errnum);
}

/* Print path to current directory */
int	builtin_pwd(t_shell *sh, t_cmd *cmd)
{
	char *buf;

	sh->sh_name = sh->sh_name; //FIX janky solution for sh not being used
	if (cmd->errnum)
		return (msg_err_ret(cmd->errnum, cmd->errname));
	buf = (char *)malloc(sizeof(char) * 1025);
	buf = getcwd(buf, 1025);
	if (!buf)
		cmd->errnum = errno;
	else
	{
		putstr_fd(buf, cmd->fd_out);
		putchar_fd('\n', cmd->fd_out);
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

	i = 1;
	while (cmd->args[i])
	{
		if (check_env_var(cmd->args[i], false))			/* Check each argument and add to env array if valid */
		{
			if (!change_env_var(&sh->env, cmd->args[i]))
				add_env_var(&sh->env, cmd->args[i]);
		}
		i++;
	}
	return (cmd->errnum);
}
