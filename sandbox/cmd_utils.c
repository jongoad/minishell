#include "parse_test.h"

t_cmd	*get_new_cmd(void)
{
	t_cmd	*cmd;

	cmd = ft_xalloc(sizeof(t_cmd));
	cmd->args = ft_xalloc(sizeof(char *));
	return (cmd);
}

t_cmd	*add_cmd(t_shell *sh)
{
	t_cmd	**new_arr;
	t_cmd	*new_cmd;
	int		i;

	sh->nb_cmds++;
	new_arr = ft_xalloc((sh->nb_cmds + 1) * sizeof(t_cmd *));
	new_cmd = ft_xalloc(sizeof(t_cmd));
	i = 0;
	while (i < sh->nb_cmds - 1)
	{
		new_arr[i] = sh->cmds[i];
		i++;
	}
	new_arr[i++] = new_cmd;
	new_arr[i] = NULL;
	free(sh->cmds);
	sh->cmds = new_arr;
	return (new_cmd);
}

void	add_cmd_arg(t_cmd *cmd, char *new_arg)
{
	char	**new_array;
	int		i;

	if (!new_arg)
		return ;
	if (!cmd->filepath)
	{
		cmd->filepath = new_arg;
		return ;
	}
	new_array = ft_xalloc((cmd->nb_args + 2) * sizeof(char *));
	i = -1;
	while (++i < cmd->nb_args)
		new_array[i] = cmd->args[i];
	new_array[i++] = new_arg;
	new_array[i] = NULL;
	cmd->nb_args++;
	free(cmd->args);
	cmd->args = new_array;
	return ;
}
