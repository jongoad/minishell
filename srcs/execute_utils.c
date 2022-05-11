#include "../includes/minishell.h"


/* Build a path for a specific command */
char	*build_cmd_path(char **cmd_path, char *cmd)
{
	char	*tmp;
	char	*cmd_res;
	int		i;

	i = 0;
	while (cmd[i])
	{
		if (cmd[0] == '.' || cmd[i] == '/')
			return (ft_strdup(cmd));
		i++;
	}
	while (*cmd_path)
	{
		tmp = ft_strjoin(*cmd_path, "/");
		cmd_res = ft_strjoin(tmp, cmd);
		free (tmp);
		if (access(cmd_res, 0) == 0)
			return (cmd_res);
		free(cmd_res);
		cmd_path++;
	}
	return (NULL);
}