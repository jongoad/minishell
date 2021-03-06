/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iyahoui- <iyahoui-@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/09 15:02:58 by jgoad             #+#    #+#             */
/*   Updated: 2022/07/01 14:07:10 by iyahoui-         ###   ########.fr       */
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

/* Initialize ms_path to get the absolute path to the minishell executable */
void	init_ms_path(t_shell *sh, char *name)
{
	char	*temp_pwd;
	char	*ptr;
	int		i;

	i = ft_strlen(name);
	while (i && name[i] != '/')
		i--;
	sh->ms_path = ft_xalloc(i + 2);
	sh->ms_path = ft_strncpy(sh->ms_path, name, i + 1);
	if (name[0] != '/')
	{
		temp_pwd = ft_strjoin(sh->pwd, "/");
		ptr = sh->ms_path;
		sh->ms_path = ft_strjoin_free(temp_pwd, sh->ms_path);
		free(ptr);
	}
}
