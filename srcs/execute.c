/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iyahoui- <iyahoui-@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/13 16:46:59 by jgoad             #+#    #+#             */
/*   Updated: 2022/06/17 15:05:30 by iyahoui-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Check return values from children and control process waiting */
static int	handle_error_return(t_shell *sh)
{
	int	stat_loc;
	int	ret;

	ret = wait(&stat_loc);
	if (ret <= 0)
		return (0);
	if (ret == sh->pids[sh->nb_cmds - 1])
	{
		if (WIFEXITED(stat_loc))
			sh->ret_val = WEXITSTATUS(stat_loc);
		if (WIFSIGNALED(stat_loc) && WTERMSIG(stat_loc) == SIGQUIT)
			printf("Quit: 3\n");
		else if (WIFSIGNALED(stat_loc) && WTERMSIG(stat_loc) == SIGINT)
			printf("\n");
		else if (WTERMSIG(stat_loc))
			sh->ret_val = 128 + WTERMSIG(stat_loc);
	}
	return (ret);
}

/* Control function for executing commands */
void	execute(t_shell *sh, bool interpret_mode)
{
	int	i;

	if (!interpret_mode)
		signal(SIGINT, void_sig);
	i = 0;
	sh->cmd_iter = 0;
	sh->pids = ft_xalloc(sizeof(pid_t) * sh->nb_cmds);
	if (init_pipes(sh))
		return ;
	while (i < sh->nb_cmds)
	{
		check_builtins(sh, sh->cmds[i]);
		if (sh->nb_cmds == 1 && sh->cmds[i]->builtin >= 0)
			sh->ret_val = run_builtin_parent(sh, sh->cmds[i]);
		else
			run_cmd(sh, sh->cmds[i], i, interpret_mode);
		close_files(sh->cmds[i]);
		i++;
		sh->cmd_iter++;
	}
	close_pipes(sh);
	if (sh->nb_cmds > 1 || (sh->nb_cmds == 1 && sh->cmds[0]->builtin < 0))
		while (handle_error_return(sh))
			;
}

/* Fork process and run a command */
void	run_cmd(t_shell *sh, t_cmd *cmd, int i, bool interpret_mode)
{
	int		ret;

	if (!interpret_mode)
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

/* Run builtin command without forking */
int	run_builtin_parent(t_shell *sh, t_cmd *cmd)
{
	if (init_io(sh, cmd))
		return (cmd->errnum);
	else
		sh->builtins.f[cmd->builtin](sh, cmd);
	return (cmd->errnum);
}
