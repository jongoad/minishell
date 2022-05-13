
#include "minishell.h"

int	main(int argc, char *argv[], char **envp)
{
	t_shell	*sh;

	sh = NULL;
	sh = init_shell(sh, argc, argv, envp);
	minishell(sh);
}


/* Container function for one iteration (loop) of shell */
int	minishell(t_shell *sh)
{

		// sh->line = ft_strdup("cat tests.txt | wc -l");
		// printf("line = %s\n", sh->line);
		// parse(sh, sh->line);
		// print_cmds_info(sh);
		// sh->pids = malloc(sizeof(pid_t) * sh->nb_cmds);
		// execute(sh);
	
	char	err_char;

	while (1)
	{
		sh->line = rl_getline(sh);
		// sh->line = ft_strdup("echo this");
		printf("line = %s\n", sh->line);
		if (!sh->line)
			exit(0) ;
		err_char = parse(sh, sh->line);
		if (err_char)
			parse_error(err_char);
		print_cmds_info(sh);
		execute(sh);

		int i = 0;
		while (i < sh->nb_cmds)
		{
			free(sh->cmds[i]);
			i++;

		}
		reset_shell(sh);
	}
	
	return (sh->ret_val);
}


