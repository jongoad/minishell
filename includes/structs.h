#ifndef STRUCTS_H
# define STRUCTS_H

# include "minishell.h"

typedef struct s_builtin
{
	int		*alias[9];		/* Array of builtin command aliases */
	void	*builtin[9];	/* Array of function pointers */
}	t_builtin;

typedef struct s_io
{
	char	**infile;		/* Path for infile, if it exists */
	char	**outfile;		/* Path for outfile, if it exists */
	int		in;				/* File descriptor for input, set to stdin by default */
	int		out;			/* File descriptor for output, set to stdout by default */
	bool	append_mode;	/* Determines whether the outfile redirection is '>>' or '>' */
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

typedef struct s_cmd
{
	t_io	io;				/* Pointer for I/O struct */
	char	**path;			/* Array of paths for current shell */
	char	**args;			/* Input arguments for the command, element 0 is the command path */
	char	**envp;			/* Environment path for the current shell */
	char	*filepath;		/* Path to command */
	int		builtin;		/* Built in command or sys command */
	int		errnum;			/* Error status (set during parsing) */
	char	*errname;		/* Name or path associated with error */
}	t_cmd;

typedef struct s_shell
{
	t_env	env;			/* Environment variables struct */
	int		*pipe;			/* Array of ints to hold pipe data */
	char	*line;			/* Buffer for line return from readlin */
	t_cmd	**cmds;			/* Array of command structs */
	pid_t	*pids;			/* Array of pid for managing waits */
	int		nb_cmds;		/* Number of commands parsed */
	int		ret_val;		/* Return value of last executed command */
}	t_shell;

#endif