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
# include "enums.h"
# include "structs.h"

/* Macro defines */

/* Error defines */
# define ERR_EXIT_ARGS_NUM ": too many arguments"
# define ERR_EXIT_NON_NUMERIC ": numeric argument required"
# define ERR_PIPE "unable to allocate pipes"

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
void		clean_cmds(t_shell *sh);
void		clean_fork(t_shell *sh, t_cmd *cmd);

/*Execution utilities */
char	*build_cmd_path(char **cmd_path, char *cmd);

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
long long	ft_atoll(const char *str, bool *is_valid);
void		add_env_var(t_env *env, char *str);
void		remove_env_var(t_env *env, int n);
bool		check_env_var(char *str, bool unset);
bool		env_var_cmp(char *arg_str, char *env_str);
void		init_builtins(t_shell *sh);
int			change_env_var(t_env *env, char *arg);

/* Pipe functions */
int			init_pipes(t_shell *sh);
void		close_pipes(t_shell *sh);
void		update_pipes(int p1, int p2);
void		manage_pipes(t_shell *sh, t_cmd *cmd);


/* Utility functions */
int			putchar_fd(char c, int fd);
int			putstr_fd(char *str, int fd);
void		put_err_msg(char *sh_name, char *cmd, char *arg, char *msg);
int			count_array(void **array);
int			abs_val(int n);
char		*ft_strdup(const char *s1);
int			ft_strncmp(const char *s1, const char *s2, size_t n);
char		*ft_strjoin(char const *s1, char const *s2);
void		free_array(void **array);
char		**ft_split(char const *s, char c);
size_t		ft_strlen(const char *s);
void		*ft_memset(void *b, int c, size_t len);
char		*str_to_lower(char *str);

/* Error handling */
int			msg_err_ret(int errnum, char *errname);
char		*get_err_msg(int errnum);

/* Memory management functions */
void		cleanup(t_shell *sh);

/* Debug functions */
void	debug_execute(t_shell *sh, char **cmds, char ***args, char ***in, char ***out);
t_cmd *create_command(t_shell *sh, char *cmd, char **args, char **in, char **out);
void	setup_debug_execute(t_shell *sh);

#endif