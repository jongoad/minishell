/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iyahoui- <iyahoui-@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/09 15:09:17 by jgoad             #+#    #+#             */
/*   Updated: 2022/07/01 13:32:25 by iyahoui-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

/* Build a path for a specific command */
char	*build_cmd_path(char **cmd_path, char *cmd)
{
	char	*tmp;
	char	*cmd_res;

	if (is_set('/', cmd) && access(cmd, X_OK) == 0)
		return (ft_strdup(cmd));
	else
	{
		while (cmd_path && *cmd_path)
		{
			tmp = ft_strjoin(*cmd_path, "/");
			cmd_res = ft_strjoin(tmp, cmd);
			free (tmp);
			if (access(cmd_res, X_OK) == 0)
				return (cmd_res);
			free(cmd_res);
			cmd_path++;
		}
	}
	return (NULL);
}

/* Check if command is builtin */
int	check_builtins(t_shell *sh, t_cmd *cmd)
{
	char	*tmp;
	int		i;

	i = 0;
	if (!cmd->exe)
		return (cmd->builtin = -1);
	tmp = ft_strdup(cmd->exe);
	tmp = str_to_lower(tmp);
	while (i < 7)
	{
		if (!ft_strncmp(tmp, sh->builtins.alias[i],
				ft_strlen(sh->builtins.alias[i])))
		{
			if (!tmp[ft_strlen(sh->builtins.alias[i])])
			{
				free(tmp);
				cmd->builtin = i;
				return (cmd->builtin);
			}
		}
		i++;
	}
	free(tmp);
	return (cmd->builtin = -1);
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

/* Fork process and run a command */
void	run_cmd(t_shell *sh, t_cmd *cmd, int i)
{
	int		ret;

	if (!sh->interpret_mode)
		signal(SIGQUIT, signal_handler);
	sh->pids[i] = fork();
	if (sh->pids[i] == 0)
	{
		if (!init_io(sh, cmd) && cmd->exe)
		{
			manage_pipes(sh, cmd);
			if (cmd->builtin < 0)
				run_cmd_external(sh, cmd);
			else
			{
				cmd->errnum = sh->builtins.f[cmd->builtin](sh, cmd);
				if (cmd->errnum)
					put_err_msg(sh->sh_name, cmd->exe, NULL, ERR_PIPE);
			}
		}
		ret = cmd->errnum;
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
