/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iyahoui- <iyahoui-@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/09 14:48:54 by jgoad             #+#    #+#             */
/*   Updated: 2022/06/13 17:09:34 by iyahoui-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Compare env var name passed to unset to record in env var  array */
bool	env_var_cmp(char *arg_str, char *env_str)
{
	int	i;

	i = 0;
	while (arg_str[i] && env_str[i] && env_str[i] != '=')
	{
		if (arg_str[i] != env_str[i])
			return (false);
		i++;
	}
	if (!arg_str[i] && env_str[i] == '=')
		return (true);
	return (false);
}

/* Print path to current directory */
char	*pwd_to_str(void)
{
	char	*buf;
	char	*tmp;

	buf = (char *)malloc(sizeof(char) * 1025);
	ft_memset((void *)buf, 0, 1025);
	buf = getcwd(buf, 1025);
	tmp = ft_strdup(buf);
	free(buf);
	return (tmp);
}

/* Handle CD error return */
int	handle_cd_error(t_shell *sh)
{
	put_err_msg("chdir", ERR_PWD, ERR_CWD, ERR_FILE);
	sh->ret_val = 1;
	return (sh->ret_val);
}

/* Handle cd command with no arguments */
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
