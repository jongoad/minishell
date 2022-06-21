/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs_bonus.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iyahoui- <iyahoui-@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/13 19:21:29 by iyahoui-          #+#    #+#             */
/*   Updated: 2022/06/21 17:37:27 by iyahoui-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_BONUS_H
# define STRUCTS_BONUS_H

# include "minishell_bonus.h"

typedef struct s_cmd	t_cmd;
typedef struct s_shell	t_shell;
typedef struct s_arglst	t_arglst;
typedef struct s_ms_job	t_ms_job;

/* Builtin command access data */
typedef struct s_builtin
{
	char	*alias[7];
	int		(*f[7])(t_shell *, t_cmd *);
}	t_builtin;

/* Readline data */
typedef struct s_read_info
{
	char		*buff;
	char		*mem_space;
	char		*line;
	int			line_len;
	int			nb_bytes_read;
}	t_read_info;

/* Linked list struct for command line tokens */
struct s_arglst
{
	char		*str;
	bool		is_env_var;
	bool		is_joined;
	int			*is_wildcard;
	t_arglst	*next;
};

/* Infile redirection data*/
typedef struct s_infile
{
	char		*infile;
	t_arglst	*in_lst;
	char		*delim;
	t_arglst	*delim_lst;
	int			fd;
}	t_infile;

/* Outfile redirection data */
typedef struct s_outfile
{
	char		*outfile;
	t_arglst	*out_lst;
	int			fd;
	bool		append_mode;
}	t_outfile;

/* Environment variables data */
typedef struct s_env
{
	char		**path;
	char		**envp;
}	t_env;

/* Individual command data */
struct s_cmd
{
	t_infile	**ins;
	t_outfile	**outs;
	char		**args;
	t_arglst	**args_lst;
	char		**envp;
	char		*exe;
	char		*errname;
	int			errnum;
	int			builtin;
	int			nb_args;
	int			nb_ins;
	int			nb_outs;
	int			fd_in;
	int			fd_out;
};

/* Main shell data */
struct s_shell
{
	t_env		env;
	t_builtin	builtins;
	t_ms_job	*jobs;
	t_cmd		**cmds;
	pid_t		*pids;
	char		*line;
	char		*prompt;
	char		*sh_name;
	char		*ms_path;
	int			*pipes;
	int			nb_cmds;
	int			nb_pipes;
	int			ret_val;
	int			cmd_iter;
	int			history_fd;
	bool		cmd_line;
	bool		interpret_mode;
	char		err_char;
	char		*pwd;
};

struct s_ms_job
{
	/* Jobs should be self-sufficient. In meantime, cmds represent strict minimum */
	t_cmd		**cmds;
	t_ms_job	*next;
	char		operator;
	int			nb_cmds;
	int			ret_val;
};

typedef struct s_wildcard
{
	char	*str;
	char	*path;
	char	**output;
	char	**search;
	int		*ends;
	int		start;
	int		end;
	int		search_tot;
	char	*ret;
	char	*tmp;
	char	*p_tmp;
	bool	ret_dot;
}	t_wildcard;

#endif