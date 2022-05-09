/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoad <jgoad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/02 15:43:30 by jgoad             #+#    #+#             */
/*   Updated: 2022/05/09 18:23:39 by jgoad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/* -------- TODO FOR EXECUTION --------

	- Deal with expansion of variables
	- Deal with closing file descriptors to prevent leaks

	- Ensure cmd->errnum is initialized to 0 for all commands prior to execution


*/

/* Control function for executing commands */
void	execute(t_shell *sh)
{
	int	i;

	i = 0;
	while (i < sh->nb_cmds)
	{
		check_builtins(sh, sh->cmds[i]);									/* Check if command is builtin, and if so get the function index */
		if (sh->nb_cmds == 1 && sh->cmds[i]->builtin >= 0)					/* If there is only one command and it is a builtin, run it without forking */
			sh->ret_val = run_builtin_parent(sh, sh->cmds[i], i);
		else
			run_cmd(sh, sh->cmds[i], i);									/* Run command(s) in forked processes */
		i++;
	}
	if (sh->nb_cmds > 1 || (sh->nb_cmds == 1 && sh->cmds[0]->builtin < 0))	/* Wait unless there was only one command and it was a builtin */
	{
		while (wait(&sh->ret_val) > 0)
			sh->ret_val >>= 8;												/* Update return value from each forked process */
	}
	clean_cmds(sh);															/* Clear command data before returning to main shell loop*/
}

/* Fork process and run a command */
void	run_cmd(t_shell *sh, t_cmd *cmd, int i)
{
	sh->pids[i] = fork();
	if (sh->pids[i] == 0)
	{
		if (init_io(sh, cmd))												/* Check IO */
		{
			msg_err_return();
			exit(cmd->errnum);
		}
		else if (cmd->builtin < 0)											/* If system command run with execve */
		{
			build_path();													/* Build file path for command */
			execve(cmd->filepath, cmd->args, sh->env.envp);
		}
		else																/* If built in command run in current process */
		{
			cmd->errnum = sh->builtins.f[cmd->builtin](sh, cmd);
			if (cmd->errnum)
				msg_err_ret(cmd->errnum, cmd->errname);
		}
		clean_fork(sh, cmd);														/* If built in function, clear memory before exit */
		exit(cmd->errnum);
	}
}

/* Init builtin commands struct */
void	init_builtins(t_shell *sh)
{
	int	i;

	i = 0;
	/* Init alias strings */
	sh->builtins.alias[0] = "echo";
	sh->builtins.alias[1] = "cd";
	sh->builtins.alias[2] = "pwd";
	sh->builtins.alias[3] = "export";
	sh->builtins.alias[4] = "unset";
	sh->builtins.alias[5] = "env";
	sh->builtins.alias[6] = "exit";

	/*Init fucntion pointer array */
	sh->builtins.f[0] = &builtin_echo;
	sh->builtins.f[1] = &builtin_cd;
	sh->builtins.f[2] = &builtin_pwd;
	sh->builtins.f[3] = &builtin_export;
	sh->builtins.f[4] = &builtin_unset;
	sh->builtins.f[5] = &builtin_env;
	sh->builtins.f[6] = &builtin_exit;
}

/* Check if command is builtin */
int	check_builtins(t_shell *sh, t_cmd *cmd)
{
	char	*tmp;
	int		i;

	i = 0;
	tmp = ft_strdup(cmd->filepath);			/* Create a local copy of command name/path */
	tmp = ft_lower(tmp);					/* Set to lowercase for comparison */

	while (i < 7)
	{
		if (!ft_strcmp(tmp, sh->builtins.alias[i]))
			return (cmd->builtin = i);
		i++;	
	}
	return (cmd->builtin = -1);
}

/* Run builtin command without forking */
int	run_builtin_parent(t_shell *sh, t_cmd *cmd, int i)
{
	sh->builtins.f[cmd->builtin](sh, cmd);
	if (cmd->errnum)
		msg_err_ret(cmd->errnum, cmd->errname);
	return (cmd->errnum);
}

/* Check input and output files and set up file descriptors */
int init_io(t_shell *sh, t_cmd *cmd)
{
	int i;

	i = -1;
	while (++i < cmd->nb_ins)
	{
		cmd->errnum = access(cmd->ins[i].infile, F_OK);
		if (!cmd->errnum)
			cmd->errnum = access(cmd->ins[i].infile, R_OK);
		if (cmd->errnum)
			return (cmd->errnum);
	}
	cmd->ins[i].fd = open(cmd->ins[i - 1].infile, O_RDONLY);
	i = -1;
	while (++i < cmd->nb_outs)
	{
		if (cmd->outs[i].append_mode)
			cmd->outs[i].fd = open(cmd->outs[i].outfile, O_CREAT | O_APPEND);
		else
			cmd->outs[i].fd = open(cmd->outs[i].outfile, O_CREAT | O_TRUNC);
		if (cmd->outs[i].fd < 0)
		{
			cmd->errnum = errno;
			return (cmd->errnum);
		}
	}
}

/* Clear all command data before leaving execution */
void	clean_cmds(t_shell *sh)
{

}

/* Clear local copy of data in fork before exiting */
void	clean_fork(t_shell *sh, t_cmd *cmd)
{

}