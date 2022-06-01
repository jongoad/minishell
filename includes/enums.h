#ifndef ENUMS_H
# define ENUMS_H

/* Parse defines */
# define WHITESPACES " \n\t"
# define CL_SPEC_CH "\"\'$"
# define CL_TOK_LIM " \n\t<>|$\"\'"		// to add: &()
# define CL_DQU_LIM " \n\t<>|$\'"		// to add: &()
# define CL_SQU_LIM " \n\t<>|$\""		// to add: &()
# define CL_DQU_NOSPEC "$\""			// to add: &()
# define CL_VAR_CHARS "_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"
# define HEREDOC_PATH "/tmp/heredoc"	// to add: &()

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