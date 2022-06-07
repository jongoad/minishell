#ifndef STRUCTS_H
# define STRUCTS_H

# include "minishell.h"

typedef struct s_cmd	t_cmd;
typedef struct s_shell	t_shell;
typedef struct s_arglst	t_arglst;

/* Builtin command access data */
typedef struct s_builtin
{
	char	*alias[7];						/* Array of builtin command aliases */
	int		(*f[7])(t_shell *, t_cmd *);	/* Array of function pointers */
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
	int			*is_wildcard; /* 0 if not wildcard, exists otherwise */
	t_arglst	*next;
};

/* Infile redirection data*/
typedef struct s_infile
{
	char		*infile;		/* Path for infile, if it exists */
	t_arglst	*in_lst;		/* Linked list of the env_vars and pure string tokens */
	char		*delim;			/* Delimiter for heredocs. NULL if infile is not a heredoc. */
	t_arglst	*delim_lst;		/* Linked list of the env_vars and pure string tokens */
	int			fd;				/* File descriptor for input, set to stdin by default */
}	t_infile;


/* Outfile redirection data */ 
typedef struct s_outfile
{
	char		*outfile;		/* Path for outfile */
	t_arglst	*out_lst;		/* Linked list of the env_vars and pure string tokens */
	int			fd;				/* File descriptor for input, set to stdin by default */
	bool		append_mode;	/* Determines whether the redir is '>' or '>>' */
}	t_outfile;


/* Environment variables data */
typedef struct s_env
{
	char		**path;			/* Path environment variable */
	char		**envp;			/* Envp array */
}	t_env;

/* Individual command data */
struct s_cmd
{
	t_infile	**ins;			/* in_files from parsing */
	t_outfile	**outs;			/* out_files from parsing */
	char		**args;			/* Input arguments for the command, element 0 is the command path */
	t_arglst	**args_lst;		/* Array of linked lists representing the command line token */
	char		**envp;			/* Environment path for the current shell */
	char		*exe;			/* Path to command */
	char		*errname;		/* Name or path associated with error */
	int			errnum;			/* Error status (set during parsing) */
	int			builtin;		/* Built in command or sys command */
	int			nb_args;		/* Number of args parsed */
	int			nb_ins;			/* Number of in_files */
	int			nb_outs;		/* Number of out_files */
	int			fd_in;			/* Final file descriptor for filestream in */
	int			fd_out;			/* Final file descriptor for filestream out */
};

// typedef struct s_job
// {
// 	t_cmd	**cmds;				/* Jobs should be self-sufficient. In meantime, cmds represent strict minimum */
// 	t_expr	*next;
// 	t_expr	*right;
// 	t_expr	*left;
// 	pid_t	*pids;
// 	char	*operator;
// 	int		nb_cmds;
// 	int		ret_val;
// }	t_job;

/* Main shell data */
struct s_shell
{
	t_env		env;			/* Environment variables struct */
	t_builtin	builtins;    	/* Builtin command struct */
	t_cmd		**cmds;			/* Array of command structs - MUST FREE */
	pid_t		*pids;			/* Array of pid for managing waits - MUST FREE */
	char		*line;			/* Buffer for line return from readlin - MUST FREE */
	char		*prompt;		/* Prompt for current shell - MUST FREE */
	char		*sh_name;		/* Name of current shell - MUST FREE */
	int			*pipes;			/* Array of ints to hold pipe data - MUST FREE */
	int			nb_cmds;		/* Number of commands parsed */
	int			nb_pipes;		/* Number of pipes created */
	int			ret_val;		/* Return value of last executed command */
	int			cmd_iter;		/* Iterator for commands array */
	int			history_fd;		/* File descriptory */
	bool		cmd_line;		/* Store presence of a command line input on program launch */
	char		err_char;		/* Error character for parse error handling */
};

typedef struct s_wildcard
{
	/* General wildcard variables */
	char	*str;
	char	*path;
	char	**output;
	char	**search;
	int		*ends;

	/* Wildcard match variables */

	int		start;
	int		end;
	int		search_tot;
	char	*ret;
	char	*tmp;
	char	*p_tmp;
}	t_wildcard;

typedef struct s_expr	t_expr;
struct s_expr
{
	t_cmd	**cmds;				/* Jobs should be self-sufficient. In meantime, cmds represent strict minimum */
	t_expr	*next;
	t_expr	*right;
	t_expr	*left;
	pid_t	*pids;
	char	*operator;
	int		nb_cmds;
	int		ret_val;
};

#endif