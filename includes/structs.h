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
	t_arglst	*next;
};

/* Infile redirection data*/
typedef struct s_infile
{
	char		*infile;		/* Path for infile, if it exists */
	t_arglst	*in_tok;		/* Linked list of the env_vars and expanded strings */
	char		*delimiter;		/* Delimiter for heredocs. NULL if infile is not a heredoc. */
	t_arglst	*del_tok;		/* Linked list of the env_vars and expanded strings */
	int			fd;				/* File descriptor for input, set to stdin by default */
}	t_infile;


/* Outfile redirection data */ 
typedef struct s_outfile
{
	char		*outfile;		/* Path for outfile */
	t_arglst	*out_tok;		/* Linked list of the env_vars and expanded strings */
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
	// IMPORTANT: args[0] needs to be set as char *exe
	char		**args;			/* Input arguments for the command, element 0 is the command path */
	t_arglst	**args_tok;		/* Linked list of the env_vars and expanded strings */
	char		**envp;			/* Environment path for the current shell */
	char		*exe;			/* Path to command */
	t_arglst	*exe_tok;		/* Linked list of the env_vars and expanded strings */
	char		*errname;		/* Name or path associated with error */
	int			errnum;			/* Error status (set during parsing) */
	int			builtin;		/* Built in command or sys command */
	int			nb_args;		/* Number of args parsed */
	int			nb_ins;			/* Number of in_files */
	int			nb_outs;		/* Number of out_files */
	int			fd_in;			/* Final file descriptor for filestream in */
	int			fd_out;			/* Final file descriptor for filestream out */
};

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
};

#endif