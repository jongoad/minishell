#include "minishell.h"

int	main(int argc, char *argv[], char **envp)
{
	t_shell	*sh;
	int		ret;

	signal(SIGINT, signal_handler);		/* Setup signal catch for SIGINT  (CTRL-C) */
	signal(SIGQUIT, signal_handler);	/* Setup signal catch for SIGQUIT (CTRL-\) */
	sh = NULL;
	sh = init_shell(sh, argv, envp);
	if (argc > 1)
	{
		if (command_line_input(sh, argc, argv))
			run_single_command(sh);
		else
			sh->cmd_line = true;
	}
	ret = minishell(sh);
	cleanup(sh);
	exit(ret);
}

/* Create parseable string from command line input */
bool	command_line_input(t_shell *sh, int argc, char **argv)
{
	bool	is_single_command;
	int		i;
	
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
		if (sh->cmds)
			execute(sh);
		ret = sh->ret_val;
	}
	cleanup(sh);
	exit(ret);
}

/* Container function for one iteration (loop) of shell */
int	minishell(t_shell *sh)
{
	while (1)
	{
		if (!sh->cmd_line)								/* Handle command line input as first execution */
			sh->line = rl_getline(sh);
		sh->cmd_line = false;
		if (!sh->line)									/* If readline returns a null line, CTRL-D has been entered. Free and exit */
			readline_exit(sh);
		sh->err_char = parse(sh, sh->line);
		if (sh->err_char)								/* Print parse error and set ret value */
			sh->ret_val = parse_error(sh->err_char);
		else
		{
			cmds_lst_to_str(sh);
			clean_linked_lists(sh);
			print_cmds_info(sh);
			if (sh->cmds)
				execute(sh);
		}
		reset_shell(sh);
	}
	return (sh->ret_val);
}

/* Handle CTRL-D exit from readline */
void	readline_exit(t_shell *sh)
{
	cleanup(sh);
	sh = NULL;
	printf("exit\n");
	exit(0);
}
