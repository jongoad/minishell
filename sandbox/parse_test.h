#ifndef PARSE_TEST_H
# define PARSE_TEST_H

# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <unistd.h>
# include <fcntl.h>
# include "./colors.h"

# define BUFFSIZE 4194304
# define WHITESPACES "\n \t"
# define CL_SPEC_CH "<>\"\'|"	// to add: &()
# define CL_TOK_LIM "<>|"		// to add: &

// STRUCTS
typedef struct s_read_info
{
	char		*buff;
	char		*mem_space;
	char		*line;
	int			line_len;
	int			nb_bytes_read;
}	t_read_info;

//NEW
typedef struct s_arglst	t_arglst;
struct s_arglst
{
	char		*str;
	bool		is_expandable;
	t_arglst	*next;
};

//END OF NEW
typedef struct s_cl_tok
{
	char	**args;
	int		n_args;
}	t_cl_tok;

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
	t_infile	**ins;		/* in_files from parsing */
	t_outfile	**outs;		/* out_files from parsing */
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
//	parse.c
int		parse_test(t_shell *sh, char *rem_line);

//	get_cl_tok.c
char	*get_cl_tok(char **line);

//	parse_utils.c
void	skip_whitespaces(char **line);

//	parse_redir.c
int		parse_redir(t_cmd *cmd, char **line);
void	add_infile(t_cmd *cmd, t_infile *new_in);
void	add_outfile(t_cmd *cmd, t_outfile *new_out);
void	parse_in(t_cmd *cmd, char *cl_tok, bool is_double);
void	parse_out(t_cmd *cmd, char *cl_tok, bool is_double);

//	cmd_utils.c
void	add_cmd_arg(t_cmd *cmd, char *arg);
t_cmd	*add_new_cmd(t_shell *sh);
t_cmd	*get_new_cmd(void);

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
void	free_split(char **split);
void	ft_putstr_fd(char *str, int fd);
int		is_set(char const c, char const *set);
int		is_set_ret(char const c, char const *set);
void	ft_bzero(void *s, size_t n);

//	print_utils.c
void	print_redirs(t_cmd *cmd);
void	print_cmd_args(t_cmd *cmd);
void	print_cmds_info(t_shell *sh);

//	arg_list.c
void		ms_lstadd(t_arglst **lst, t_arglst *new);
t_arglst	*ms_lstnew(char *str, bool is_expandable);
void		ms_lstclear(t_arglst **lst);
void		ms_lstdelone(t_arglst *lst);

#endif