#ifndef PARSE_TEST_H
# define PARSE_TEST_H

# include "../includes/structs.h"
# include "../includes/minishell.h"
# include "../includes/enums.h"

# define BUFFSIZE 4194304
# define WHITESPACES "\n \t"

// STRUCTS
typedef struct s_read_info
{
	char		*buff;
	char		*mem_space;
	char		*line;
	int			line_len;
	int			nb_bytes_read;
}	t_read_info;


typedef struct s_io
{
	char	**infiles;		/* Path for infile, if it exists */
	char	**outfiles;		/* Path for outfile, if it exists */
	int		in;				/* File descriptor for input, set to stdin by default */
	int		out;			/* File descriptor for output, set to stdout by default */
	bool	append_mode;
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
	char	**args;			/* Input arguments for the command, element 0 is the command path */
	char	**envp;			/* Environment path for the current shell */
	char	*filepath;		/* Path to command */
	char	*errname;		/* Name or path associated with error */
	int		errnum;			/* Error status (set during parsing) */
	int		builtin;		/* Built in command or sys command */
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


// FUNCTIONS
//	init_test.c
t_shell	*init_shell(void);

//	cleanup_test.c
void	cleanup_shell(t_shell *shell);

//	getline.c
char	*get_line(int fd);

//	getline_utils.c
char	*ft_strncpy(char *dest, char *src, size_t n);
void	*ft_xalloc(size_t size);
size_t	ft_strlen(const char *s);
size_t	strlen_c(char *s, char c);

//	utils.c
void	free_split(void **split);

#endif