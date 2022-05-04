#ifndef ENUMS_H
# define ENUMS_H

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