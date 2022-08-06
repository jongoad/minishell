/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iyahoui- <iyahoui-@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/09 14:43:36 by jgoad             #+#    #+#             */
/*   Updated: 2022/08/06 18:46:03 by iyahoui-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Create a new env var for current shell instance */
int	builtin_export(t_shell *sh, t_cmd *cmd)
{
	int	i;

	i = 0;
	if (!cmd->args[1])
	{
		while (sh->env.envp[i])
		{
			printf("%s%s\n", "declare -x ", sh->env.envp[i]);
			i++;
		}
		return (cmd->errnum);
	}
	i = 1;
	while (cmd->args[i])
	{
		if (check_env_var(cmd->args[i], false))
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

/* Unset env var for current shell instance */
int	builtin_unset(t_shell *sh, t_cmd *cmd)
{
	int	i;
	int	j;

	i = 0;
	while (cmd->args[++i])
	{
		if (!check_env_var(cmd->args[i], true))
		{
			put_err_msg(sh->sh_name, cmd->exe, cmd->args[i], ERR_IDENTIFER);
			cmd->errnum = 1;
		}
		else
		{
			j = -1;
			while (sh->env.envp[++j])
			{
				if (env_var_cmp(cmd->args[i], sh->env.envp[j]))
				{
					remove_env_var(&sh->env, j);
					break ;
				}
			}
		}
	}
	return (cmd->errnum);
}

/* Print out env vars for current shell instance */
int	builtin_env(t_shell *sh, t_cmd *cmd)
{
	int	i;

	i = 0;
	while (sh->env.envp[i])
	{
		putstr_fd(sh->env.envp[i], cmd->fd_out);
		putchar_fd('\n', cmd->fd_out);
		i++;
	}
	return (cmd->errnum);
}

/* Exit current shell instance */
int	builtin_exit(t_shell *sh, t_cmd *cmd)
{
	bool		is_valid;
	long long	ret;

	ret = 0;
	putstr_fd("exit\n", 2);
	if (cmd->args[1])
	{
		ret = ft_atoll(cmd->args[1], &is_valid);
		if (is_valid && cmd->args[2])
		{
			put_err_msg(sh->sh_name, cmd->args[0], NULL, ERR_EXIT_ARGS_NUM);
			cmd->errnum = 1;
			return (1);
		}
		else if (!is_valid)
		{
			ret = 255;
			put_err_msg(sh->sh_name, cmd->args[0],
				cmd->args[1], ERR_EXIT_NON_NUMERIC);
		}
	}
	cleanup(sh, true);
	exit(ret);
	return (ret);
}
