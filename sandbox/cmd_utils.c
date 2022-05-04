#include "parse_test.h"

t_cmd	*get_new_cmd(void)
{
	t_cmd	*cmd;

	cmd = ft_xalloc(sizeof(t_cmd));
	cmd->args = ft_xalloc(sizeof(char *));
	cmd->io.infiles = ft_xalloc(sizeof(char *));
	cmd->io.outfiles = ft_xalloc(sizeof(char *));
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

void	add_cmd_arg(t_cmd *cmd, char *arg)
{
	t_cmd	**new_array;
	t_cmd	*new_arg;
	int		i;

	cmd->nb_args++;
	new_array = ft_xalloc((cmd->nb_args + 1) * sizeof(char *));
	i = 0;
	while (i < cmd->nb_args)
	{
		new_array[i] = cmd->args[i];
		i++;
	}
	new_array[i++] = new_arg;
	new_array[i] = NULL;
	free(cmd->args);
	cmd->args = new_array;
	return (new_arg);
}
