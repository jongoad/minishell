#include "minishell.h"

/* Signal handler function */
void	signal_handler(int signum)
{
	if (signum == SIGINT)
	{
		printf("\n");
		rl_replace_line("", 1);
		rl_on_new_line();
		rl_redisplay();
	}
	if (signum == SIGQUIT || signum == SIGTSTP)
	{
		rl_replace_line("", 1);
		rl_on_new_line();
		rl_redisplay();
	}
}	
