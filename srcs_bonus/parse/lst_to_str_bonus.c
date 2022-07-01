/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_to_str_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iyahoui- <iyahoui-@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/13 16:54:58 by jgoad             #+#    #+#             */
/*   Updated: 2022/07/01 13:25:25 by iyahoui-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

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
		if (cmd->args_lst && cmd->args_lst[0]->str[0] == '(')
			expand_subshell_cmd(sh, cmd);
		else if (cmd->args_lst)
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
