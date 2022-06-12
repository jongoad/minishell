#ifndef ENUMS_H
# define ENUMS_H

/* Parse defines */
# define CL_SPEC_CH "\"\'$"
# define CL_SAME_TOK "\"\'$/"
# define WHITESPACES " \n\t\v\r\f"
# define CL_TOK_LIM " \n\t\v\r\f<>|$\"\'"		// to add: &()
# define CL_VAR_LIM " \n\t\v\r\f<>|\"\'"		// to add: &()
# define CL_DQU_LIM " \n\t\v\r\f<>|$\'"		// to add: &()
# define CL_SQU_LIM " \n\t\v\r\f<>|$\""		// to add: &()
# define CL_DQU_NOSPEC "$\""			// to add: &()
# define ENV_VAR_CHARS "_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"
# define CL_TOK_LIM_BONUS "* \n\t\v\r\f<>|$\"\'"
# define CL_SPEC_CH_BONUS "*\"\'$"
# define HEREDOC_PATH "/tmp/heredoc"	// to add: &()
# define HIST_LOG ".minishell_history"	// to add: &()
# define WILDCARD 26

# ifndef BONUS
#  define BONUS 0
# endif

/* Parsing result flags */
enum parse
{
	INVALID = 0,
	CMD = 1,
	REDIR = 2,
	ENV_VAR = 3,
	INFILE = 4,
	OUTFILE = 5
};

/* Builtin command flags */
enum builtin
{
	E_ECHO = 0,
	E_CD = 1,
	E_PWD = 2,
	E_EXPORT = 3,
	E_UNSET = 4,
	E_ENV = 5,
	E_EXIT = 6
};

#endif