#include "minishell.h"

int	test_minishell(t_shell *sh);

int	main(int argc, char *argv[], char **envp)
{
	t_shell	*sh;

	sh = NULL;
	sh = init_shell(sh, argc, argv, envp);
	if (argc > 1)
		test_minishell(sh);
	else
		minishell(sh);
	cleanup(sh);
	return (sh->ret_val);
}

int	test_minishell(t_shell *sh)
{
	char	err_char;

	// sh->line = ft_strdup("\"lol\"abc$SHLVL");
	sh->line = ft_strdup("$SHLVL\"o\"\'ooo\' e\"lol\" \"\"\"lol$HELL\"$SHLVL \'sq\'\"dq\" \"unclosed");
	printf("line = %s\n", sh->line);
	err_char = parse(sh, sh->line);
	if (err_char)
		parse_error(err_char);
	cmds_lst_to_str(sh);
	print_cmds_info(sh);
	// sh->pids = malloc(sizeof(pid_t) * sh->nb_cmds);
	// execute(sh);
	free (sh->line);
	return (0);
}

/* Container function for one iteration (loop) of shell */
int	minishell(t_shell *sh)
{
	char	err_char;

	while (1)
	{
		sh->line = rl_getline(sh);
		if (!sh->line)
		{
			putendl_fd("exit", STDERR_FILENO);
			exit(0);
		}
		err_char = parse(sh, sh->line);
		if (err_char)
			parse_error(err_char);
		cmds_lst_to_str(sh);
		print_cmds_info(sh);
		execute(sh);
		reset_shell(sh);
	}
	return (sh->ret_val);
}


