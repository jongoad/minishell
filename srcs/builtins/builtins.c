/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iyahoui- <iyahoui-@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/09 14:46:44 by jgoad             #+#    #+#             */
/*   Updated: 2022/06/12 16:36:555 by iyahoui-         ###   ########.fr       */
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

int	handle_cd_error(t_shell *sh)
{
	// fct_err_msg("chdir", strerror(errno));
	put_err_msg("chdir", ERR_PWD, ERR_CWD, ERR_FILE);
	sh->ret_val = 1;
	return (sh->ret_val);
}

void	cd_no_arg(t_shell *sh, t_cmd *cmd)
{
	char	**new_arr;
	int		i;

	new_arr = ft_xalloc((cmd->nb_args + 2) * sizeof(char *));
	i = 0;
	while (i < cmd->nb_args)
	{
		new_arr[i] = cmd->args[i];
		i++;
	}
	new_arr[i] = expand_env_var(sh->env.envp, "HOME");
	cmd->args = new_arr;
	cmd->nb_args += 1;
	return ;
}

/* Change directory */
int	builtin_cd(t_shell *sh, t_cmd *cmd)
{
	char *buf;
	char *res;

	buf = (char *)malloc(sizeof(char) * 1025);
	buf = getcwd(buf, 1025);
	if (!buf)
		return (handle_cd_error(sh));
	res = ft_strjoin("OLDPWD=", buf);
	free(buf);
	if (!change_env_var(&sh->env, res))
		add_env_var(&sh->env, ft_strjoin("OLDPWD=", res));
	free(res);
	if (cmd->nb_args == 1 || (cmd->args[1][0] == '~' && !cmd->args[1][1]))
		cd_no_arg(sh, cmd);
	if (chdir(cmd->args[cmd->nb_args - 1]))
	{
		cmd->errnum = errno;
		cmd->errname = ft_strjoin("cd: ", cmd->args[cmd->nb_args - 1]);
		put_err_msg(sh->sh_name, cmd->exe, cmd->args[cmd->nb_args - 1], ERR_FILE);
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
		put_err_msg(cmd->exe, ERR_PWD, ERR_CWD, ERR_FILE);
		return (cmd->errnum);
	}
	return (cmd->errnum);
}

/* Create a new env var for current shell instance */
int	builtin_export(t_shell *sh, t_cmd *cmd)
{
	int	i;
	int	j;

	j = 0;
	if (!cmd->args[1])								 	/* If  no arguments passed print out declare list */
	{
		while (sh->env.envp[j])
		{
			printf("%s%s\n", "declare -x ", sh->env.envp[j]);
			j++;
		}
		return(cmd->errnum);
	}
	i = 1;
	while (cmd->args[i])
	{
		if (check_env_var(cmd->args[i], false))			/* Check each argument and add to env array if valid */
		{	
			if (!change_env_var(&sh->env, cmd->args[i]))
				add_env_var(&sh->env, cmd->args[i]);
		}
		else
			put_err_msg(sh->sh_name, cmd->exe, cmd->args[i], ERR_IDENTIFER);
		i++;
	}
	return (cmd->errnum);
}
