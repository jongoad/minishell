/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoad <jgoad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/09 14:43:36 by jgoad             #+#    #+#             */
/*   Updated: 2022/06/09 14:48:46 by jgoad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Unset env var for current shell instance */
int	builtin_unset(t_shell *sh, t_cmd *cmd)
{
	int	i;
	int	j;

	i = 1;
	while (cmd->args[i])
	{
		if (!check_env_var(cmd->args[i], true))					/* If arg to be unset does not pass env var naming conventions print error */
		{
			put_err_msg(sh->sh_name, cmd->exe, cmd->args[i], ERR_IDENTIFER);
			cmd->errnum = 1;
		}
		else
		{
			j = 0;
			while (sh->env.envp[j])								/* Find and remove variable if it exists */
			{
				if (env_var_cmp(cmd->args[i], sh->env.envp[j]))
					remove_env_var(&sh->env, j);
				j++;
			}
		}
		i++;
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
	if (cmd->args[1])
	{
		ret = ft_atoll(cmd->args[1], &is_valid);
		if (is_valid && cmd->args[2])
		{
			putstr_fd("exit\n", 2);
			put_err_msg(sh->sh_name, cmd->args[0], NULL, ERR_EXIT_ARGS_NUM);
			return (1);
		}
		else if (!is_valid)
		{
			ret = 255;
			put_err_msg(sh->sh_name, cmd->args[0], cmd->args[1], ERR_EXIT_NON_NUMERIC);
		}
		else
			putstr_fd("exit\n", 2);
	}
	else
		putstr_fd("exit\n", 2);
	cleanup(sh);						/* Cleanup ALL shell memory before exit */
	exit(ret);
}
