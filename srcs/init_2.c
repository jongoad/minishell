/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iyahoui- <iyahoui-@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/09 15:02:58 by jgoad             #+#    #+#             */
/*   Updated: 2022/06/21 16:58:26 by iyahoui-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Update shell level for current shell */
char	*increment_shlvl(char *str)
{
	char	*tmp;
	char	*num;

	while (*str && *str != '=')
		str++;
	str++;
	num = ft_itoa(ft_atoi(str) + 1);
	tmp = ft_strjoin("SHLVL=", num);
	free(num);
	return (tmp);
}

/* Set PWD upon shell launch */
void	init_pwd(t_shell *sh)
{
	int		i;
	char	*tmp;

	i = 0;
	while (sh->env.envp[i])
	{
		if (!ft_strncmp(sh->env.envp[i], "PWD=", 4))
		{
			tmp = pwd_to_str();
			printf("getcwd = %s\n", tmp);
			if (!tmp)
			{
				put_err_msg("minishell-init", ERR_PWD, ERR_CWD, ERR_FILE);
				free(tmp);
				return ;
			}
			free(sh->env.envp[i]);
			sh->env.envp[i] = ft_strjoin("PWD=", tmp);
			free(sh->pwd);
			sh->pwd = ft_strdup(tmp);
			free(tmp);
			return ;
		}
		i++;
	}
}

/* Init builtin commands struct */
void	init_builtins(t_shell *sh)
{
	sh->builtins.alias[0] = "echo";
	sh->builtins.alias[1] = "cd";
	sh->builtins.alias[2] = "pwd";
	sh->builtins.alias[3] = "export";
	sh->builtins.alias[4] = "unset";
	sh->builtins.alias[5] = "env";
	sh->builtins.alias[6] = "exit";
	sh->builtins.f[0] = &builtin_echo;
	sh->builtins.f[1] = &builtin_cd;
	sh->builtins.f[2] = &builtin_pwd;
	sh->builtins.f[3] = &builtin_export;
	sh->builtins.f[4] = &builtin_unset;
	sh->builtins.f[5] = &builtin_env;
	sh->builtins.f[6] = &builtin_exit;
}
