#ifndef STRUCTS_H
# define STRUCTS_H

# include "minishell.h"

typedef struct s_cmd	t_cmd;
typedef struct s_shell	t_shell;
typedef struct s_builtin
{
	char	*alias[7];							/* Array of builtin command aliases */
	void	(*f[7])(t_shell *, t_cmd *);	/* Array of function pointers */
}	t_builtin;

typedef struct s_read_info
{
	char		*buff;
	char		*mem_space;
	char		*line;
	int			line_len;
	int			nb_bytes_read;
}	t_read_info;

typedef struct s_infile
{
	char	*infile;		/* Path for infile, if it exists */
	char	*delimiter;		/* Delimiter for heredocs. NULL if infile is not a heredoc. */
	int		fd;				/* File descriptor for input, set to stdin by default */
}	t_infile;

typedef struct s_outfile
{
	char	*outfile;		/* Path for infile, if it exists */
	int		fd;				/* File descriptor for input, set to stdin by default */
	bool	append_mode;	/* Determines whether the redir is '>' or '>>' */
}	t_outfile;

typedef struct s_io
{
	char	**outfiles;		/* Path for outfile, if it exists */
	int		out;			/* File descriptor for output, set to stdout by default */
	int		open_mode;		/* O_APPEND | O_RDWR | O_RDONLY | O_WRONLY | HEREDOC */
}	t_io;

typedef struct s_env
{
	char	**path;			/* Path environment variable */
	char	**envp;			/* Envp array */
	char	*pwd;			/* Current working directory of the shell */
	char	*home;			/* Root directory for current user */
	char	*user;			/* User name of current user */
	int		shlvl;			/* The depth of the current shell instance */
}	t_env;

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
};

struct s_shell
{
	t_env		env;			/* Environment variables struct */
	int			*pipe;			/* Array of ints to hold pipe data */
	char		*line;			/* Buffer for line return from readlin */
	t_cmd		**cmds;			/* Array of command structs */
	pid_t		*pids;			/* Array of pid for managing waits */
	int			nb_cmds;		/* Number of commands parsed */
	int			ret_val;		/* Return value of last executed command */
	t_builtin	builtins;     /* Builtin command struct */
};


#endif