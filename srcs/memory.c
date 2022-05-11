#include "../includes/minishell.h"

/* Run for a complete clean of all memory */
void	cleanup(t_shell *sh)
{
	free(sh);
	//Free cmd structs
	//Free shell struct
}

void	clean_cmds(t_shell *sh)
{
	close_pipes(sh);
	//Free cmd structs
	//Free pids
	//Free pipes
}
//FIX need to free all command data

/* Free a null terminated 2d array */
void	free_array(void **array)
{
	if (array)
	{
		if (array[0])
		{
			free(array[1]);
			free(array[0]);
		}
		free(array);
	}
}