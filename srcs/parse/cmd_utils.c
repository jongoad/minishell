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

/**!
 * 	NOTE: do we want arg[0] to be dupped, or do we simply want argll
 * 			to be converted to cmd->exe at lst_to_str() ?
 * 
 * 			It doesn't make sense to parse the line twice
 * 	
 */
void	add_cmd_exe(t_cmd *cmd, char **line)
{
	set_cl_tok(&cmd->exe_tok, line);
	cmd->args_tok = ft_xalloc(2 * sizeof(t_arglst *));
	cmd->args_tok[0] = cmd->exe_tok;
	cmd->args_tok[1] = NULL;
	cmd->nb_args++;
}

void	add_cmd_arg(t_cmd *cmd, char **line)
{
	t_arglst	**new_arr;
	t_arglst	*lst;
	int			i;

	if (!*line)
		return ;
	if (!cmd->exe_tok)
		return (add_cmd_exe(cmd, line));
	new_arr = ft_xalloc((cmd->nb_args + 2) * sizeof(t_arglst *));
	i = -1;
	while (++i < cmd->nb_args)
		new_arr[i] = cmd->args_tok[i];
	lst = NULL;
	// if (i > 0)
	// 	lst = cmd->args_tok[i];
	set_cl_tok(&lst, line);
	new_arr[i++] = lst;
	new_arr[i] = NULL;
	free(cmd->args_tok);
	cmd->args_tok = new_arr;
	cmd->nb_args++;

	// char	**new_array;
	// int		i;
	// new_array = ft_xalloc((cmd->nb_args + 2) * sizeof(char *));
	// i = -1;
	// while (++i < cmd->nb_args)
	// 	new_array[i] = cmd->args[i];
	// new_array[i++] = new_arg;
	// new_array[i] = NULL;
	// free(cmd->args);
	// cmd->args = new_array;
	return ;
}
