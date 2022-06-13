/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iyahoui- <iyahoui-@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/09 14:46:44 by jgoad             #+#    #+#             */
/*   Updated: 2022/06/13 17:22:12 by iyahoui-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Echo string */
int	builtin_echo(t_shell *sh, t_cmd *cmd)
{
	int	i;

	i = 1;
	if (cmd->args[1] && !ft_strncmp(cmd->args[1], "-n", 3))
		return (builtin_echo_n(sh, cmd));
	while (cmd->args[i])
	{
		putstr_fd(cmd->args[i], cmd->fd_out);
		if (cmd->args[i + 1])
			putchar_fd(' ', cmd->fd_out);
		i++;
	}
	putchar_fd('\n', cmd->fd_out);
	return (cmd->errnum);
}

/* Echo string without newline */
int	builtin_echo_n(t_shell *sh, t_cmd *cmd)
{
	int	i;

	i = 2;
	while (cmd->args[i] && !ft_strncmp(cmd->args[i], "-n", 3))
		i++;
	while (cmd->args[i])
	{
		putstr_fd(cmd->args[i], cmd->fd_out);
		if (cmd->args[i + 1])
			putchar_fd(' ', cmd->fd_out);
		i++;
	}
	sh->ret_val = cmd->errnum;
	return (cmd->errnum);
}

/* Change directory */
int	builtin_cd(t_shell *sh, t_cmd *cmd)
{
	char	*buf;
	char	*res;

	buf = NULL;
	res = NULL;
	if (!sh->pwd)
	{
		buf = (char *)malloc(sizeof(char) * 1025);
		res = getcwd(buf, 1025);
		if (!res)
		{
			if (cmd->nb_args == 1)
				cd_no_arg(sh, cmd);
			chdir(cmd->args[cmd->nb_args - 1]);
			handle_cd_error(sh);
		}
		res = getcwd(buf, 1025);
		free(sh->pwd);
		sh->pwd = ft_strdup(res);
		free(buf);
		return (sh->ret_val);
	}
	builtins_cd_2(sh, cmd, res, buf);
	return (cmd->errnum);
}

/* Extension for CD function */
void	builtins_cd_2(t_shell *sh, t_cmd *cmd, char *res, char *buf)
{
	res = ft_strjoin("OLDPWD=", sh->pwd);
	if (!change_env_var(&sh->env, res))
		add_env_var(&sh->env, ft_strjoin("OLDPWD=", res));
	free(res);
	if (cmd->nb_args == 1 || (cmd->args[1][0] == '~' && !cmd->args[1][1]))
		cd_no_arg(sh, cmd);
	if (chdir(cmd->args[cmd->nb_args - 1]))
	{
		cmd->errnum = errno;
		cmd->errname = ft_strjoin("cd: ", cmd->args[cmd->nb_args - 1]);
		put_err_msg(sh->sh_name, cmd->exe,
			cmd->args[cmd->nb_args - 1], ERR_FILE);
	}
	else
	{
		buf = (char *)malloc(sizeof(char) * 1025);
		buf = getcwd(buf, 1025);
		res = ft_strjoin("PWD=", buf);
		free(sh->pwd);
		sh->pwd = buf;
		change_env_var(&sh->env, res);
		free(res);
	}
}

/* Print path to current directory */
int	builtin_pwd(t_shell *sh, t_cmd *cmd)
{
	char	*buf;

	sh->sh_name = sh->sh_name;
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
		put_err_msg(cmd->exe, ERR_PWD, ERR_CWD, ERR_FILE);
		return (cmd->errnum);
	}
	return (cmd->errnum);
}
