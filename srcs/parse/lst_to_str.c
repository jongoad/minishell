#include "minishell.h"

/**
 * @brief 
 * 			We need to decide how we want to store $?
 * 			In practice, expand_env_vars should be called at exec_time
 */
char	*expand_env_var(char **envp, char *var_name)
{
	int		var_len;
	int		i;

	if (!envp || !var_name)
		return (NULL);
	var_len = ft_strlen(var_name);
	i = -1;
	while (envp[++i])
		if (!ft_strncmp(envp[i], var_name, var_len) && envp[i][var_len] == '=')
			return (ft_strdup(&(envp[i][var_len + 1])));
	return (NULL);
}

char	*lst_to_str(t_env *env, t_arglst *lst)
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
			curr_arg = expand_env_var(env->envp, ptr->str);
		else
			curr_arg = ft_strdup(ptr->str);
		str = ft_strjoin_free(str, curr_arg);
		ptr = ptr->next;
	}
	return (str);
}

char	**lst_arr_to_str_arr(t_env *env, t_arglst **lst_arr, int nb_elems)
{
	char	**str_arr;
	int		i;

	if (!lst_arr || !*lst_arr || nb_elems < 1)
		return (NULL);
	str_arr = ft_xalloc((nb_elems + 1) * sizeof(char *));
	i = 0;
	while (i < nb_elems)
	{
		str_arr[i] = lst_to_str(env, lst_arr[i]);
		i++;
	}
	return (str_arr);
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
		cmd->exe = lst_to_str(&sh->env, cmd->args_lst[0]);
		cmd->args = lst_arr_to_str_arr(&sh->env, cmd->args_lst, cmd->nb_args);
		// cmd->args = 
		// j = -1;
		// while (++j < cmd->nb_args)
		// 	cmd->args[j] = lst_to_str(&sh->env, cmd->args[j]->in_lst);
		j = -1;
		while (++j < cmd->nb_ins)
			cmd->ins[j]->infile = lst_to_str(&sh->env, cmd->ins[j]->in_lst);
		j = -1;
		while (++j < cmd->nb_outs)
			cmd->outs[j]->outfile = lst_to_str(&sh->env, cmd->outs[j]->out_lst);
		i++;
	}
}
