/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iyahoui- <iyahoui-@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/13 15:57:48 by jgoad             #+#    #+#             */
/*   Updated: 2022/06/20 14:48:19 by iyahoui-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

int	main(int argc, char *argv[], char **envp)
{
	t_shell	*sh;
	int		ret;

	// signal(SIGINT, signal_handler);
	// signal(SIGQUIT, SIG_IGN);
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

	if (!ft_strncmp(argv[1], "-c", 2) && !argv[1][2])
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
		sh->line = ft_strjoin_free(sh->line, " ");
		sh->line = ft_strjoin_free(sh->line, argv[i]);
		i++;
	}
	return (is_single_command);
}

/* Run a single command and exit shell */
void	run_single_command(t_shell *sh)
{
	int		ret;

	ret = validate_input(sh->line);
	if (ret)
		ret = parse_error(ret);
	ret = parse(sh, sh->line);
	if (ret)
		ret = parse_error(ret);
	else
	{
		cmds_lst_to_str(sh);
		if (sh->cmds)
			execute(sh, true);
		ret = sh->ret_val;
	}
	cleanup(sh);
	exit(ret);
}

/* Container function for one iteration (loop) of shell */
int	minishell(t_shell *sh)
{
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		if (!sh->cmd_line)
			sh->line = rl_getline(sh);
		sh->cmd_line = false;
		if (!sh->line)
			readline_exit(sh);
		sh->err_char = validate_input(sh->line);
		printf("%s:%d : ret = %d\n", __FUNCTION__, __LINE__, sh->err_char);
		if (sh->err_char)
			sh->ret_val = parse_error(sh->err_char);
		sh->err_char = parse_jobs(sh, sh->line);
		// sh->err_char = parse(sh, sh->line);
		// if (sh->err_char)
		// 	sh->ret_val = parse_error(sh->err_char);
		// else
		// {
		// 	cmds_lst_to_str(sh);
		// 	// print_cmds_info(sh);
		// 	if (sh->cmds)
		// 		execute(sh, false);
		// }
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
