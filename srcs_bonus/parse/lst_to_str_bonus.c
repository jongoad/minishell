/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_to_str_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iyahoui- <iyahoui-@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/13 16:54:58 by jgoad             #+#    #+#             */
/*   Updated: 2022/06/23 16:24:00 by iyahoui-         ###   ########.fr       */
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

// void	parse_parenthesis(t_arglst **lst, char **line)
// {
// 	t_shell	*sh;
// 	// char	*exe;
// 	char	*arg;
// 	char	*temp;
// 	int		parenthesis_len;

// 	sh = get_data();
// 	arg = ft_strjoin(sh->ms_path, sh->sh_name);
// 	// ms_lstadd(lst, ms_lstnew(arg, false));
// 	arg = ft_strjoin_free(arg, " -c ");
// 	// ms_lstadd(lst, ms_lstnew(arg, false));
// 	printf("%s:%d : *line = `%s\'\n", __FUNCTION__, __LINE__, *line);
// 	printf("%s:%d : added exe = `%s\'\n", __FUNCTION__, __LINE__, arg);
// 	printf("%s:%d : parenthesis len: `%d\'\n", __FUNCTION__, __LINE__, parenthesis_len);
// 	parenthesis_len = get_parenthesis_len(*line);
// 	temp = ft_xalloc(parenthesis_len);
// 	temp = ft_strncpy(temp, *line + 1, parenthesis_len - 1);
// 	arg = ft_strjoin_free(arg, temp);
// 	free(temp);
// 	temp = arg;
// 	printf("%s:%d : parenthesis contents: `%s\'\n", __FUNCTION__, __LINE__, temp);
// 	printf("%s:%d : entire arg = `%s\'\n", __FUNCTION__, __LINE__, arg);
// 	ms_lstadd(lst, ms_lstnew(arg, false));
// 	*line += parenthesis_len + 1;
// 	return ;
// }

void	expand_subshell_cmd(t_shell *sh, t_cmd *cmd)
{
	// char	*parenthesis_contents;
	char	**str_arr;
	int		parenthesis_len;

	cmd->nb_args = 3;
	str_arr = ft_xalloc(4 * sizeof(char *));
	str_arr[0] = ft_strjoin(sh->ms_path, sh->sh_name);
	str_arr[1] = ft_strdup("-c");
	parenthesis_len = get_parenthesis_len(cmd->args_lst[0]->str);
	str_arr[2] = ft_xalloc(parenthesis_len);
	str_arr[2] = ft_strncpy(str_arr[2], cmd->args_lst[0]->str + 1, parenthesis_len - 1);
	str_arr[3] = NULL;
	cmd->args = str_arr;
	cmd->exe = ft_strdup(cmd->args[0]);
	return ;
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
