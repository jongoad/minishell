#ifndef MINISHELL_H
# define MINISHELL_H

/* System includes */
# include <dirent.h>
# include <curses.h>
# include <term.h>
# include <stdlib.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/types.h>
# include <stdio.h>
# include <sys/stat.h>
# include <sys/ioctl.h>
# include <termios.h>
# include <signal.h>
# include <fcntl.h>
# include <string.h>
# include <sys/wait.h>
# include <sys/time.h>
# include <sys/resource.h>
# include <stdbool.h>
# include <errno.h>

/* User includes */
# include "colors.h"
# include "enums.h"
# include "structs.h"
# include "library.h"
# include "error.h"
# include "get_next_line.h"

/* Parse defines */
# define WHITESPACES "\n \t"
# define CL_SPEC_CH "<>\"\'|"	// to add: &()
# define CL_TOK_LIM "<>|"		// to add: &

/* Function prototypes */

/* Main control functions */
int			minishell(t_shell *sh);

/* Initialization functions */
t_shell		*init_shell(t_shell *sh, int argc, char **argv, char **envp);
void		init_shell_prompt(t_shell *sh, char *name);
void		init_env_vars(t_shell *sh, char **envp);
void		init_builtins(t_shell *sh);

/* Execution functions */
void		execute(t_shell *sh);
void		run_cmd(t_shell *sh, t_cmd *cmd, int i);
int			check_builtins(t_shell *sh, t_cmd *cmd);
int			run_builtin_parent(t_shell *sh, t_cmd *cmd);
int			init_io(t_shell *sh, t_cmd *cmd);
int			init_ins(t_shell *sh, t_cmd *cmd);
int			init_outs(t_shell *sh, t_cmd *cmd);
void		close_files(t_cmd *cmd);

/*Execution utilities */
char		*build_cmd_path(char **cmd_path, char *cmd);

/* Builtin commands */
int			builtin_echo(t_shell *sh, t_cmd *cmd);
int			builtin_echo_n(t_shell *sh, t_cmd *cmd);
int			builtin_cd(t_shell *sh, t_cmd *cmd);
int			builtin_pwd(t_shell *sh, t_cmd *cmd);
int			builtin_export(t_shell *sh, t_cmd *cmd);
int			builtin_unset(t_shell *sh, t_cmd *cmd);
int			builtin_env(t_shell *sh, t_cmd *cmd);
int			builtin_exit(t_shell *sh, t_cmd *cmd);

/* Builtin command utilities */
void		add_env_var(t_env *env, char *str);
void		remove_env_var(t_env *env, int n);
bool		check_env_var(char *str, bool unset);
bool		env_var_cmp(char *arg_str, char *env_str);
void		init_builtins(t_shell *sh);
int			change_env_var(t_env *env, char *arg);

/* Pipe functions */
int			init_pipes(t_shell *sh);
void		close_pipes(t_shell *sh);
void		manage_pipes(t_shell *sh, t_cmd *cmd);

/* Error handling */
void		put_err_msg(char *sh_name, char *cmd, char *arg, char *msg);
int			msg_err_ret(int errnum, char *errname);
char		*get_err_msg(int errnum);
int			parse_error(char err_char);

/* Memory management functions */
void		cleanup(t_shell *sh);
void		clean_cmds(t_shell *sh);
void		clean_fork(t_shell *sh, t_cmd *cmd);
void		reset_shell(t_shell *sh);

/* Parsing function */

//	parse.c
int		parse(t_shell *sh, char *rem_line);
int		check_parse(t_shell *sh, t_cmd *cmd, char *line);

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

//	print_utils.c
void	print_redirs(t_cmd *cmd);
void	print_cmd_args(t_cmd *cmd);
void	print_cmds_info(t_shell *sh);

//	arg_list.c
void		ms_lstadd(t_arglst **lst, t_arglst *new);
t_arglst	*ms_lstnew(char *str, bool is_expandable);
void		ms_lstclear(t_arglst **lst);
void		ms_lstdelone(t_arglst *lst);

/* Readline functions */
char	*rl_getline(t_shell *sh);
void	init_history(t_shell *sh);

#endif