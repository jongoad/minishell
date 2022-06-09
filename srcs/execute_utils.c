#include "minishell.h"

/* Build a path for a specific command */
char	*build_cmd_path(char **cmd_path, char *cmd)
{
	char	*tmp;
	char	*cmd_res;

	if (is_set('/', cmd) && access(cmd, X_OK) == 0)
		return (ft_strdup(cmd));
	else
	{
		while (cmd_path && *cmd_path)
		{
			tmp = ft_strjoin(*cmd_path, "/");
			cmd_res = ft_strjoin(tmp, cmd);
			free (tmp);
			if (access(cmd_res, X_OK) == 0)
				return (cmd_res);
			free(cmd_res);
			cmd_path++;
		}
	}
	return (NULL);
}

/* Check if command is builtin */
int	check_builtins(t_shell *sh, t_cmd *cmd)
{
	char	*tmp;
	int		i;

	i = 0;
	if(!cmd->exe)							/* If there is no command return -1 here to prevent segfault */
		return (cmd->builtin = -1);
	tmp = ft_strdup(cmd->exe);				/* Create a local copy of command name/path */
	tmp = str_to_lower(tmp);				/* Set to lowercase for comparison */
	while (i < 7)
	{
		if (!ft_strncmp(tmp, sh->builtins.alias[i], ft_strlen(sh->builtins.alias[i])))
		{
			if (!tmp[ft_strlen(sh->builtins.alias[i])])
			{
				free(tmp);
				return (cmd->builtin = i);
			}
		}
		i++;	
	}
	free(tmp);
	return (cmd->builtin = -1);
}