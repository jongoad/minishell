#ifndef STRUCTS_H
# define STRUCTS_H

# include "minishell.h"

typedef struct s_cmd	t_cmd;
typedef struct s_shell	t_shell;

/* Builtin command access data */
typedef struct s_builtin
{
	char	*alias[7];							/* Array of builtin command aliases */
	int		(*f[7])(t_shell *, t_cmd *);		/* Array of function pointers */
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

/* Infile redirection data*/
typedef struct s_infile
{
	char	*infile;		/* Path for infile, if it exists */
	char	*delimiter;		/* Delimiter for heredocs. NULL if infile is not a heredoc. */
	int		fd;				/* File descriptor for input, set to stdin by default */
}	t_infile;

/* Outfile redirection data*/ 
typedef struct s_outfile
{
	char	*outfile;	 	/* Path for oufile, if it exists */
	int		fd;				/* File descriptor for output, set to stdout by default */
	bool	append_mode;	/* Determines whether the redir is '>' or '>>' */
}	t_outfile;

/* Environment variables data */
typedef struct s_env
{
	char	**path;			/* Path environment variable */
	char	**envp;			/* Envp array */
}	t_env;

/* Individual command data */
struct s_cmd
{
	t_infile	*ins;		/* in_files from parsing */
	t_outfile	*outs;		/* out_files from parsing */
	char		**args;		/* Input arguments for the command, element 0 is the command path */
	char		**envp;		/* Environment path for the current shell */
	char		*filepath;	/* Path to command */
	char		*errname;	/* Name or path associated with error */
	int			errnum;		/* Error status (set during parsing) */
	int			builtin;	/* Built in command or sys command */
	//	NEW
	int			nb_args;	/* Number of args parsed */
	int			nb_ins;		/* Number of in_files */
	int			nb_outs;	/* Number of out_files */
	int			fd_in;		/* Final file descriptor for filestream in */
	int			fd_out;		/* Final file descriptor for filestream out */
};

/* Main shell data */
struct s_shell
{
	t_env		env;			/* Environment variables struct */
	int			*pipes;			/* Array of ints to hold pipe data - MUST FREE */
	int			nb_pipes;		/* Number of pipes created */
	int			cmd_iter;		/* Iterator for commands array (must start at 1 not 0, or needs to be dealth with for pipes) */
	char		*line;			/* Buffer for line return from readlin - MUST FREE */
	t_cmd		**cmds;			/* Array of command structs - MUST FREE */
	pid_t		*pids;			/* Array of pid for managing waits - MUST FREE */
	int			nb_cmds;		/* Number of commands parsed */
	int			ret_val;		/* Return value of last executed command */
	t_builtin	builtins;    	/* Builtin command struct */
	char		*prompt;		/* Prompt for current shell - MUST FREE */
	char		*sh_name;		/* Name of current shell - MUST FREE */
};


#endif