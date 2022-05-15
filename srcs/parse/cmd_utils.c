#include "minishell.h"

t_cmd	*get_new_cmd(void)
{
	t_cmd	*cmd;

	cmd = ft_xalloc(sizeof(t_cmd));
	cmd->args = ft_xalloc(sizeof(char *));
	return (cmd);
}

t_cmd	*add_new_cmd(t_shell *sh)
{
	t_cmd	**new_arr;
	t_cmd	*new_cmd;
	int		i;

	new_arr = ft_xalloc((sh->nb_cmds + 2) * sizeof(t_cmd *));
	new_cmd = ft_xalloc(sizeof(t_cmd));
	i = -1;
	while (++i < sh->nb_cmds)
		new_arr[i] = sh->cmds[i];
	new_arr[i++] = new_cmd;
	new_arr[i] = NULL;
	free(sh->cmds);
	sh->cmds = new_arr;
	sh->nb_cmds++;
	return (new_cmd);
}

void	add_cmd_arg(t_cmd *cmd, char **line)
{
	t_arglst	**new_arr;
	t_arglst	*lst;
	int			i;

	if (!*line)
		return ;
	new_arr = ft_xalloc((cmd->nb_args + 2) * sizeof(t_arglst *));
	i = -1;
	while (++i < cmd->nb_args)
		new_arr[i] = cmd->args_lst[i];
	lst = NULL;
	set_cl_tok(&lst, line);
	new_arr[i++] = lst;
	new_arr[i] = NULL;
	free(cmd->args_lst);
	cmd->args_lst = new_arr;
	cmd->nb_args++;
	return ;
}
