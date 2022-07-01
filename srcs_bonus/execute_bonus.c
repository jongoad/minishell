/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iyahoui- <iyahoui-@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/13 16:46:59 by jgoad             #+#    #+#             */
/*   Updated: 2022/07/01 13:33:26 by iyahoui-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

static void	update_cmds(t_shell *sh, t_ms_job *ptr)
{
	sh->cmds = ptr->cmds;
	sh->nb_cmds = ptr->nb_cmds;
}

void	execute_jobs(t_shell *sh)
{
	t_ms_job	*ptr;
	t_ms_job	*prev;
	int			ret;
	char		operator;

	ptr = sh->jobs;
	operator = 0;
	prev = NULL;
	while (ptr)
	{
		update_cmds(sh, ptr);
		if (!prev || (ret && operator == '|') || (!ret && operator == '&'))
		{
			execute(sh);
			ret = sh->ret_val;
		}
		operator = ptr->operator;
		prev = ptr;
		ptr = ptr->next;
		free(prev);
		prev = NULL;
		sh->jobs = ptr;
		reset_shell(sh);
	}
	sh->jobs = NULL;
}

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
void	execute(t_shell *sh)
{
	int	i;

	if (!sh->interpret_mode)
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
			run_cmd(sh, sh->cmds[i], i);
		close_files(sh->cmds[i]);
		i++;
		sh->cmd_iter++;
	}
	close_pipes(sh);
	if (sh->nb_cmds > 1 || (sh->nb_cmds == 1 && sh->cmds[0]->builtin < 0))
		while (handle_error_return(sh))
			;
}
