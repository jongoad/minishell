/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_bonus.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iyahoui- <iyahoui-@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/13 19:29:54 by iyahoui-          #+#    #+#             */
/*   Updated: 2022/07/01 13:12:47 by iyahoui-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_BONUS_H
# define MINISHELL_BONUS_H

/* System includes */
# include <dirent.h>
# include <curses.h>
# include <term.h>
# include <stdlib.h>
# include <unistd.h>
// # include <readline/readline.h>
// # include <readline/history.h>
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
# include <limits.h>

/* Readline includes */
# include "readline.h"
# include "history.h"

/* User includes */
# include "colors_bonus.h"
# include "enums_bonus.h"
# include "structs_bonus.h"
# include "library_bonus.h"
# include "error_bonus.h"
# include "get_next_line_bonus.h"

/* Function prototypes */
void		signal_handler(int signum);
void		void_sig(int sig);
t_shell		*get_data(void);
void		readline_exit(t_shell *sh);

/* Main control functions */
int			minishell(t_shell *sh);

/* Initialization functions */
t_shell		*init_shell(t_shell *sh, char **argv, char **envp);
void		init_shell_prompt(t_shell *sh, char *name);
void		init_env_vars(t_shell *sh, char **envp);
char		*increment_shlvl(char *str);
void		init_builtins(t_shell *sh);
void		init_path(t_shell *sh);
void		init_ms_path(t_shell *sh, char *name);
void		init_pwd(t_shell *sh);

/* Execution functions */
void		execute_jobs(t_shell *sh);
void		execute(t_shell *sh);
void		run_cmd(t_shell *sh, t_cmd *cmd, int i);
void		run_cmd_external(t_shell *sh, t_cmd *cmd);
int			check_builtins(t_shell *sh, t_cmd *cmd);
int			run_builtin_parent(t_shell *sh, t_cmd *cmd);
int			init_io(t_shell *sh, t_cmd *cmd);
int			init_ins(t_shell *sh, t_cmd *cmd);
int			init_outs(t_shell *sh, t_cmd *cmd);
void		close_files(t_cmd *cmd);

/* Command line exectution */
bool		command_line_input(t_shell *sh, int argc, char **argv);
void		run_single_command(t_shell *sh);

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
void		builtins_cd_2(t_shell *sh, t_cmd *cmd, char *res, char *buf);
int			handle_cd_error(t_shell *sh);
void		cd_no_arg(t_shell *sh, t_cmd *cmd);
void		add_env_var(t_env *env, char *str);
void		remove_env_var(t_env *env, int n);
bool		check_env_var(char *str, bool unset);
bool		env_var_cmp(char *arg_str, char *env_str);
void		init_builtins(t_shell *sh);
int			change_env_var(t_env *env, char *arg);
char		*pwd_to_str(void);

/* Pipe functions */
int			init_pipes(t_shell *sh);
void		close_pipes(t_shell *sh);
void		manage_pipes(t_shell *sh, t_cmd *cmd);

/* Error handling */
void		put_err_msg(char *sh_name, char *cmd, char *arg, char *msg);
int			msg_err_ret(int errnum, char *errname);
int			fct_err_msg(char *fct, char *errname);
char		*get_err_msg(int errnum);
int			parse_error(char err_char);

/* Memory management functions */
void		cleanup(t_shell *sh);
void		clean_env(t_shell *sh);
void		clean_linked_lists(t_shell *sh);
void		clean_single_cmd_linked_lists(t_cmd *cmd);
void		clean_single_cmd(t_cmd *cmd);
void		clean_input(t_cmd *cmd);
void		clean_output(t_cmd *cmd);
void		clean_cmds(t_shell *sh);
void		clean_fork(t_shell *sh, t_cmd *cmd);
void		reset_shell(t_shell *sh);
void		clean_jobs(t_shell *sh);

/* Parsing functions */

//	validate.c
int			validate_input(char *line);

//	parse.c
int			parse(t_shell *sh, char *rem_line);
int			check_parse(t_shell *sh, t_cmd *cmd, char *line);

//	parse_jobs.c
int			parse_jobs(t_shell *sh, char *line);

//	job_lst_utils.c
t_ms_job	*ms_jobnew(t_cmd **cmds, int nb_cmds, char operator);
t_ms_job	*ms_joblast(t_ms_job *lst);
void		ms_jobadd(t_ms_job **lst, t_ms_job *new);
void		ms_jobclear(t_ms_job **lst);
void		ms_jobdelone(t_ms_job *lst);

//	tokenizer.c
void		tokenize(t_arglst **lst, char **line);
void		add_to_token(t_arglst **lst, char **line);

//	parse_token.c
void		parse_dquotes(t_arglst **lst, char **line);
void		parse_squotes(t_arglst **lst, char **line);
void		parse_env_var(t_arglst **lst, char **line);
void		parse_wildcard(t_arglst **lst, char **line);
void		parse_parenthesis(t_arglst **lst, char **line);

//	add_token.c
void		add_token(
				t_arglst **lst, char **line, char *delim, bool is_env_var);
void		add_token_by_set(
				t_arglst **lst, char **line, char *set, bool is_env_var);
void		add_token_by_len(
				t_arglst **lst, char **line, int tok_len, bool is_env_var);

//	heredoc.c
void		read_heredoc(t_cmd *cmd, t_infile *in);
char		*get_heredoc_filename(void);
void		parse_heredoc(t_cmd *cmd, t_infile *in);
void		close_heredoc(int signum);
char		*expand_heredoc_tok(char **envp, char **ptr);
void		expand_heredoc(t_cmd *cmd, t_infile *in, char *heredoc);

//	lst_to_str.c
char		*expand_env_var(char **envp, char *var_name);
char		*lst_to_str(char **envp, t_arglst *lst);
char		*lst_to_str_no_exp(t_arglst *lst);
char		**lst_arr_to_str_arr(char **envp, t_arglst **arglst, int nb_elems);
void		cmds_lst_to_str(t_shell *sh);

//	parse_utils.c
void		skip_whitespaces(char **line);
int			skip_quotes(char **line);
int			get_tok_len(char *token, char *delimiters);
int			get_tok_len_set(char *token, char *set);
int			get_parenthesis_len(char *line);

//	parse_redir.c
int			parse_redir(t_cmd *cmd, char **line);
void		add_infile(t_cmd *cmd, t_infile *new_in);
void		add_outfile(t_cmd *cmd, t_outfile *new_out);
void		parse_in(t_cmd *cmd, char **line, bool is_double);
void		parse_out(t_cmd *cmd, char **line, bool is_double);

//	cmd_utils.c
void		add_cmd_arg(t_cmd *cmd, char **line);
t_cmd		*add_new_cmd(t_shell *sh);
t_cmd		*get_new_cmd(void);

//	print_utils.c
void		print_ins(t_cmd *cmd);
void		print_outs(t_cmd *cmd);
void		print_cmd_args(t_cmd *cmd);
void		print_cmds_info(t_shell *sh);
void		print_char_arr(char **arr);

//	args_list.c
void		ms_lstadd(t_arglst **lst, t_arglst *new);
t_arglst	*ms_lstnew(char *str, bool is_expandable);
void		ms_lstclear(t_arglst **lst);
void		ms_lstdelone(t_arglst *lst);
t_arglst	*ms_lstlast(t_arglst *lst);

/* Readline functions */
char		*rl_getline(t_shell *sh);
void		init_history(t_shell *sh);

/* Wildcard functions */
void		check_wildcards_debug(t_shell *sh);
void		check_wildcard(t_cmd *cmd);
char		**join_array_array(char **arr1, char **arr2);
char		**expand_wildcard(char *arg);
void		check_ends(char *str, int *ends);
char		**search_directory(char **direct, char **search, int *ends);
bool		is_wildcard_match(char *direct, char **search, int *ends);
int			get_search_tot(char **search);
char		**read_directory(t_wildcard *wc, char *path);
char		**add_str_array(char **array, char *str);
void		split_path_wildcard(t_wildcard *wc, char *arg);
void		add_path_wildcard(char **results, char *path);
char		*ft_strjoin_free_rev(char *s1, char *s2);
bool		free_return_bool(char *str, bool ret);
char		**wildcard_error_return(t_wildcard *wc);
void		init_wildcard(t_wildcard *wc, char *arg);
bool		check_no_wildcard(t_cmd *cmd);
char		*replace_wildcard(char *str);
bool		is_wildcard_match_ends(t_wildcard *wc, char **search, int *ends);

#endif