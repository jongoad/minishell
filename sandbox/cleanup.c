#include "./parse_test.h"

void	cleanup_shell(t_shell *sh)
{
	int	i;	

	i = 0;
	while (i < sh->nb_cmds)
	{
		free_split(sh->cmds[i]->args);
		free(sh->cmds[i]->filepath);
		free(sh->cmds[i]->errname);
	}
	free (sh->line);
	free (sh->pids);
	return ;
}
