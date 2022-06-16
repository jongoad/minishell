/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_to_str.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iyahoui- <iyahoui-@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/13 16:54:58 by jgoad             #+#    #+#             */
/*   Updated: 2022/06/16 18:00:57 by iyahoui-         ###   ########.fr       */
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

/* Convert linked list to struct array format */
char	*lst_to_str(char **envp, t_arglst *lst)
{
	t_arglst	*ptr;
	char		*str;
	char		*curr_arg;

	if (!lst)
		return (NULL);
	str = NULL;
	ptr = lst;
	while (ptr)
	{
		if (ptr->is_env_var)
			curr_arg = expand_env_var(envp, ptr->str);
		else
			curr_arg = ft_strdup(ptr->str);
		str = ft_strjoin_free(str, curr_arg);
		free(curr_arg);
		ptr = ptr->next;
	}
	return (str);
}

/* Convert linked list to struct array without expansion */
char	*lst_to_str_no_exp(t_arglst *lst)
{
	t_arglst	*ptr;
	char		*str;
	char		*curr_arg;

	if (!lst)
		return (NULL);
	str = NULL;
	ptr = lst;
	while (ptr)
	{
		if (ptr->is_env_var)
			curr_arg = ft_strjoin("$", ptr->str);
		else
			curr_arg = ft_strdup(ptr->str);
		str = ft_strjoin_free(str, curr_arg);
		free(curr_arg);
		ptr = ptr->next;
	}
	return (str);
}

/* Expand and convert command arguments from linked list format */
void	expand_cmd_args(t_shell *sh, t_cmd *cmd)
{
	char	*arg_str;
	char	**str_arr;
	int		i;
	int		j;
	int		nb_args;

	nb_args = cmd->nb_args;
	str_arr = ft_xalloc((nb_args + 1) * sizeof(char *));
	j = 0;
	i = -1;
	while (++i < nb_args)
	{
		arg_str = lst_to_str(sh->env.envp, cmd->args_lst[i]);
		if (cmd->args_lst[i]->is_joined)
		{
			str_arr[j] = ft_strjoin_free(str_arr[j], arg_str);
			free(arg_str);
			arg_str = NULL;
			cmd->nb_args -= 1;
		}
		else
			str_arr[j++] = arg_str;
	}
	cmd->args = str_arr;
	cmd->exe = ft_strdup(cmd->args[0]);
}

/* Convert linked list command to struct array format */
void	cmds_lst_to_str(t_shell *sh)
{
	t_cmd	*cmd;
	int		i;
	int		j;

	i = 0;
	while (i < sh->nb_cmds)
	{
		cmd = sh->cmds[i];
		if (cmd->args_lst)
			expand_cmd_args(sh, cmd);
		j = -1;
		while (++j < cmd->nb_ins)
			if (cmd->ins[j]->infile == NULL)
				cmd->ins[j]->infile
					= lst_to_str(sh->env.envp, cmd->ins[j]->in_lst);
		j = -1;
		while (++j < cmd->nb_outs)
			cmd->outs[j]->outfile
				= lst_to_str(sh->env.envp, cmd->outs[j]->out_lst);
		i++;
	}
}
