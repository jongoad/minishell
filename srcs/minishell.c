#include "minishell.h"

int	test_minishell(t_shell *sh);

int	main(int argc, char *argv[], char **envp)
{
	t_shell	*sh;
	int		ret;

	signal(SIGINT, signal_handler);		/* Setup signal catch for SIGINT  (CTRL-C) */
	signal(SIGQUIT, signal_handler);	/* Setup signal catch for SIGQUIT (CTRL-\) */
	signal(SIGTSTP, signal_handler);	/* Setup signal catch for SIGTSTP (CTRL-Z) */

	sh = NULL;
	sh = init_shell(sh, argc, argv, envp);
	// if (argc > 1)
	// 	test_minishell(sh);
	if (argc > 1)
	{
		if (command_line_input(sh, argc, argv))
			run_single_command(sh);
		else
			sh->cmd_line = true;
	}
	ret = minishell(sh);
	cleanup(sh);
	return (ret);
}

/* Create parseable string from command line input */ 
bool	command_line_input(t_shell *sh, int argc, char **argv)
{
	bool	is_single_command;
	int 	i;
	if (!ft_strncmp(argv[1], "-c", 2) && !argv[1][2])	//If first argument is -c, run a single command then exit
	{
		is_single_command = true;
		i = 2;
	}
	else
	{
		is_single_command = false;
		i = 1;
	}
	
	while (i < argc)
	{
		sh->line = ft_strjoin_free(sh->line, " ");		/* Add a space to seperate each argument */
		sh->line = ft_strjoin_free(sh->line, argv[i]);	/* Join argument */
		i++;
	}
	return (is_single_command);
}

/* Run a single command and exit shell */
void	run_single_command(t_shell *sh)
{
	int		ret;

	ret = parse(sh, sh->line);
	if (ret)
	{
		parse_error(ret);
		ret = 1;
	}
	else
	{
		cmds_lst_to_str(sh);
		check_wildcards_debug(sh);
		print_cmds_info(sh);
		// if (sh->cmds)
		// 	execute(sh);
		ret = sh->ret_val;
	}
	cleanup(sh);
	exit(ret);
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
	while (1)
	{
		if (!sh->cmd_line)					/* Handle command line input as first execution */
			sh->line = rl_getline(sh);
		sh->cmd_line = false;
		if (!sh->line)						/* If readline returns a null line, CTRL-D has been entered. Free and exit */
			readline_exit(sh);
		sh->err_char = parse(sh, sh->line);
		if (sh->err_char)					/* Print parse error and set ret value */
			sh->ret_val = parse_error(sh->err_char);
		else
		{
			cmds_lst_to_str(sh);
			// print_cmds_info(sh);
			if (sh->cmds)
				execute(sh);
		}
		reset_shell(sh);
	}
	return (sh->ret_val);
}

/* Handle CTRL-D exit from readline */
void readline_exit(t_shell *sh)
{
	cleanup(sh);
	sh = NULL;
	printf("exit\n");
	exit(0);
}
