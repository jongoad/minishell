
#include "../includes/minishell.h"

int	main(int argc, char *argv[], char *envp[])
{
	t_shell	*sh;

	sh = get_data();
	init_shell(&sh, envp);

	while (minishell(sh))
		;
		
	clean(sh);
}



/* Container function for one iteration (loop) of shell */
int	minishell(t_shell *sh)
{
	char *buf;
	buf = readline();
	parse();
	execute();
	return (sh->ret_val);
}
