#include "minishell.h"

char	*expand_env_var(t_env *env, char *var_name)
{
	char	*curr_var;
	char	*env_var;
	int		curr_strlen;
	int		var_strlen;
	int		i;

	if (!env || !var_name)
		return (NULL);
	var_strlen = ft_strlen(var_name);
	env_var = NULL;
	i = 0;
	while (env->envp[i])
	{
		curr_var = get_first_token(env->envp[i], '=');
		curr_strlen = ft_strlen(curr_var);
		if (var_strlen == curr_strlen && !ft_strncmp(curr_var, var_name, var_strlen))
		{
			env_var = ft_strdup(&(env->envp[i][curr_strlen + 1]));
			break ;
		}
		free(curr_var);
		i++;
	}
	return (env_var);
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
			curr_arg = expand_env_var(env, ptr->str);
		else
			curr_arg = ft_strdup(ptr->str);
		str = ft_strjoin_free(str, curr_arg);
		ptr = ptr->next;
	}
	return (str);
}

char	**lst_arr_to_str_arr(t_env *env, t_arglst **arglst, int nb_elems)
{
	char	**str_arr;
	int		i;

	if (!arglst || !*arglst || nb_elems < 1)
		return (NULL);
	str_arr = ft_xalloc((nb_elems + 1) * sizeof(char *));
	i = 0;
	while (i < nb_elems)
	{
		str_arr[i] = lst_to_str(env, arglst[i]);
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
		cmd->exe = lst_to_str(&sh->env, cmd->exe_tok);
		cmd->args = lst_arr_to_str_arr(&sh->env, cmd->args_tok, cmd->nb_args);
		j = -1;
		while (++j < cmd->nb_ins)
			cmd->ins[j]->infile = lst_to_str(&sh->env, cmd->ins[j]->in_lst);
		j = -1;
		while (++j < cmd->nb_outs)
			cmd->outs[j]->outfile = lst_to_str(&sh->env, cmd->outs[j]->out_lst);
		i++;
	}
}
