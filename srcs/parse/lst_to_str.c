#include "minishell.h"

/**
 * @brief 
 * 			We need to decide how we want to store $?
 * 			In practice, expand_env_vars should be called at exec_time
 */
char	*expand_env_var(char **envp, char *var_name)
{
	t_shell	*sh;
	char	*expanded;
	int		var_len;
	int		i;

	if (!envp || !var_name)
		return (NULL);
	/* Expand error return */
	sh = get_data();
	if (var_name[0] == '?' && !var_name[1])
		return (ft_itoa(sh->ret_val));
	/* Expand wildcards */
	if (BONUS && var_name[0] == '*' && !var_name[1])
	{
		expanded = malloc(2);
		expanded[0] = WILDCARD;
		expanded[1] = '\0';
		return (expanded);
	}
	/* Search for the variable */
	var_len = ft_strlen(var_name);
	i = -1;
	while (envp[++i])
		if (!ft_strncmp(envp[i], var_name, var_len) && envp[i][var_len] == '=')
			return (ft_strdup(&(envp[i][var_len + 1])));
	return (NULL);
}

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
		ptr = ptr->next;
	}
	return (str);
}

char	**lst_arr_to_str_arr(char **envp, t_arglst **lst_arr, int nb_elems)
{
	char	**str_arr;
	int		i;
	// int		j;

	if (!lst_arr || !*lst_arr || nb_elems < 1)
		return (NULL);
	str_arr = ft_xalloc((nb_elems + 1) * sizeof(char *));
	i = 0;
	while (i < nb_elems)
	{
		str_arr[i] = lst_to_str(envp, lst_arr[i]);
		// if (str_arr[j])
		// 	j++;
		// else

		i++;
	}
	str_arr[i] = NULL;
	return (str_arr);
}

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

void	cmds_lst_to_str(t_shell *sh)
{
	t_cmd	*cmd;
	int		i;
	int		j;

	i = 0;
	while (i < sh->nb_cmds)
	{
		cmd = sh->cmds[i];
		if (cmd->args_lst)		/* If args_lst is null there is no command, do not attempt to access */
		{
			expand_cmd_args(sh, cmd);
			// cmd->args = ft_xalloc((cmd->nb_args + 1) * sizeof(char *));
			// j = 0;
			// while (i < cmd->nb_args)
			// {
			// 	cmd->args[j] = lst_to_str(sh->env.envp, cmd->args_lst[i]);
			// 	if (cmd->args[j])
			// 		j++;
			// 	else
			// 		cmd->nb_args--;
			// 	i++;
			// }
			// cmd->args = lst_arr_to_str_arr(sh->env.envp, cmd->args_lst, cmd->nb_args);
			// cmd->exe = lst_to_str(sh->env.envp, cmd->args_lst[0]);
		}
		j = -1;
		while (++j < cmd->nb_ins)
			cmd->ins[j]->infile = lst_to_str(sh->env.envp, cmd->ins[j]->in_lst);
		j = -1;
		while (++j < cmd->nb_outs)
			cmd->outs[j]->outfile = lst_to_str(sh->env.envp, cmd->outs[j]->out_lst);
		i++;
	}
	// print_cmds_info(sh);
}
