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
	ECHO = 0,
	ECHO_N = 1,
	CD = 2,
	PWD = 3,
	EXPORT = 4,
	UNSET = 5,
	ENV = 6,
	EXIT = 7
};

#endif