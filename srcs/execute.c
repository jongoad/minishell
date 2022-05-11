/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoad <jgoad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/02 15:43:30 by jgoad             #+#    #+#             */
/*   Updated: 2022/05/11 18:38:30 by jgoad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/* -------- TODO FOR EXECUTION --------

	- Deal with expansion of variables
	- Ensure cmd->errnum is initialized to 0 for all commands prior to execution
	- Ensure all fds are initialized to -1
	- Ensure all array are null terminated??

*/






/* Control function for executing commands */
void	execute(t_shell *sh)
{
	int	i;

	i = 0;
	sh->cmd_iter = 1;														/* Iterator for pipe control */
	if (init_pipes(sh))
	{
		clean_cmds(sh);	//Prob also want to set ret value
		return;
	}
	while (i < sh->nb_cmds)
	{
		check_builtins(sh, sh->cmds[i]);									/* Check if command is builtin, and if so get the function index */
		if (sh->nb_cmds == 1 && sh->cmds[i]->builtin >= 0)					/* If there is only one command and it is a builtin, run it without forking */
			sh->ret_val = run_builtin_parent(sh, sh->cmds[i]);
		else
			run_cmd(sh, sh->cmds[i], i);									/* Run command(s) in forked processes */
		i++;
		sh->cmd_iter++;
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
		manage_pipes(sh, cmd);
		if (!init_io(sh, cmd))													/* Check IO */
		{
			if (cmd->builtin < 0)												/* If system command run with execve */
			{
				execve(build_cmd_path(sh->env.path, cmd->filepath), cmd->args, sh->env.envp);
			}
			else																/* If built in command run in current process */
			{
				cmd->errnum = sh->builtins.f[cmd->builtin](sh, cmd);
				if (cmd->errnum)
					put_err_msg(sh->sh_name, cmd->filepath, NULL, ERR_PIPE);		/* Check getting correct error message here */
			}
		}
		clean_fork(sh, cmd);													/* If built in function, clear memory before exit */
		exit(cmd->errnum);
	}
}

/* Check if command is builtin */
int	check_builtins(t_shell *sh, t_cmd *cmd)
{
	char	*tmp;
	int		i;

	i = 0;
	tmp = ft_strdup(cmd->filepath);			/* Create a local copy of command name/path */
	tmp = str_to_lower(tmp);				/* Set to lowercase for comparison */

	while (i < 7)
	{
		if (!ft_strncmp(tmp, sh->builtins.alias[i], ft_strlen(sh->builtins.alias[i])))
		{
			if (!tmp[ft_strlen(sh->builtins.alias[i])])
				return (cmd->builtin = i);
		}
		i++;	
	}
	return (cmd->builtin = -1);
}

/* Run builtin command without forking */
int	run_builtin_parent(t_shell *sh, t_cmd *cmd)
{
	if (init_io(sh, cmd))
		return (cmd->errnum);
	else
	{
		sh->builtins.f[cmd->builtin](sh, cmd);
		if (cmd->errnum)
			put_err_msg(sh->sh_name, cmd->filepath, NULL, NULL);		/* Check getting correct error message here */
	}
	return (cmd->errnum);
}

/* Check input and output files and set up file descriptors */
int init_io(t_shell *sh, t_cmd *cmd)
{
	if (init_ins(sh, cmd))
		return (cmd->errnum);
	if (init_outs(sh, cmd))
		return (cmd->errnum);
	return (cmd->errnum);
}
//All file descriptors should be initialized to -1 to start so we know what needs to be closed when error occurs

/* Check input redirections and set up file descriptor */
int	init_ins(t_shell *sh, t_cmd *cmd)
{
	int i;

	i = -1;
	while (++i < cmd->nb_ins)
	{
		cmd->errnum = access(cmd->ins[i].infile, F_OK);
		if (!cmd->errnum)
			cmd->errnum = access(cmd->ins[i].infile, R_OK);
		if (cmd->errnum)
		{
			put_err_msg(sh->sh_name, cmd->ins[i].infile, NULL, strerror(cmd->errnum));
			close_files(cmd); //Maybe move
			return (cmd->errnum);
		}
	}
	cmd->ins[i - 1].fd = open(cmd->ins[i - 1].infile, O_RDONLY);
	cmd->fd_in = cmd->ins[i - 1].fd;
	return (cmd->errnum);
}

/* Intialize outfiles */
int	init_outs(t_shell *sh, t_cmd *cmd)
{
	int	i;

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
			put_err_msg(sh->sh_name, cmd->ins[i].infile, NULL, strerror(cmd->errnum));
			close_files(cmd); //Maybe move
			return (cmd->errnum);
		}
		//Ismael change
		if (i)
			close (cmd->outs[i - 1].fd);
	}
	cmd->fd_out = cmd->outs[i - 1].fd;
	return (cmd->errnum);
}

/* Close all open file descriptors */
void	close_files(t_cmd *cmd)
{
	int i;

	i = 0;
	while (i < cmd->nb_ins)
	{
		if (cmd->ins[i].fd != -1)
			close(cmd->ins[i].fd);
		i++;
	}
	i = 0;
	while (i < cmd->nb_outs)
	{
		if (cmd->outs[i].fd != -1)
			close(cmd->outs[i].fd);
		i++;
	}
}

/* Clear local copy of data in fork before exiting */
void	clean_fork(t_shell *sh, t_cmd *cmd)
{
	close_files(cmd);							/* Close all open file descriptors */
	cleanup(sh);								/* Free all memory allocated in shell struct and its child structs */
}