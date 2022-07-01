/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_tokens_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iyahoui- <iyahoui-@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/13 16:54:58 by jgoad             #+#    #+#             */
/*   Updated: 2022/07/01 13:31:27 by iyahoui-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

/* Expand an environment variable to its true value */
char	*expand_env_var(char **envp, char *var_name)
{
	t_shell	*sh;
	char	*expanded;
	int		var_len;
	int		i;

	if (!envp || !var_name)
		return (NULL);
	sh = get_data();
	if (var_name[0] == '?' && !var_name[1])
		return (ft_itoa(sh->ret_val));
	if (var_name[0] == '*' && !var_name[1])
	{
		expanded = malloc(2);
		expanded[0] = WILDCARD;
		expanded[1] = '\0';
		return (expanded);
	}
	var_len = ft_strlen(var_name);
	i = -1;
	while (envp[++i])
		if (!ft_strncmp(envp[i], var_name, var_len) && envp[i][var_len] == '=')
			return (ft_strdup(&(envp[i][var_len + 1])));
	return (NULL);
}

void	expand_subshell_cmd(t_shell *sh, t_cmd *cmd)
{
	char	**str_arr;
	int		parenthesis_len;

	cmd->nb_args = 3;
	str_arr = ft_xalloc(4 * sizeof(char *));
	str_arr[0] = ft_strjoin(sh->ms_path, sh->sh_name);
	str_arr[1] = ft_strdup("-c");
	parenthesis_len = get_parenthesis_len(cmd->args_lst[0]->str);
	str_arr[2] = ft_xalloc(parenthesis_len);
	str_arr[2] = ft_strncpy(
			str_arr[2], cmd->args_lst[0]->str + 1, parenthesis_len - 1);
	str_arr[3] = NULL;
	cmd->args = str_arr;
	cmd->exe = ft_strdup(cmd->args[0]);
	return ;
}
