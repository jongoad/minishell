#include "minishell.h"

/* Signal handler function */
void	signal_handler(int signum)
{
	t_shell	*sh;

	sh = get_data();
	if (signum == SIGINT)
	{
		printf("\n");
		rl_replace_line("", 1);
		rl_on_new_line();
		if (!sh->cmds)
		{
			rl_redisplay();
			sh->ret_val = 1;
		}
	}
	if (signum == SIGQUIT)
	{
		if (sh->cmds)
		{
			printf("Quit: %d\n", signum);
			rl_replace_line("", 1);
			rl_on_new_line();
		}
		else
		{
			rl_on_new_line();
			rl_redisplay();
		}
	}
}	
