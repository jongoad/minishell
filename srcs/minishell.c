
#include "../includes/minishell.h"

int	main(int argc, char *argv[], char *envp[])
{
	t_shell	*sh;

	sh = NULL;
	sh = init_shell(sh, argc, argv, envp);
	


	minishell(sh);
	// while (minishell(sh))
	// 	;
		
	// clean(sh);
}



/* Container function for one iteration (loop) of shell */
int	minishell(t_shell *sh)
{
	// char *buf;
	// buf = readline(sh->prompt);
	// parse();

	setup_debug_execute(sh);
	execute(sh);
	return (sh->ret_val);
}
