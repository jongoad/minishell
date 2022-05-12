#include "parse_test.h"

int	check_parse(t_shell *sh, t_cmd *cmd, char *line)
{
	printf("entered check_parse\n");
	printf("line : \"%s\"\n", line);
	if (!cmd->ins && !cmd->outs && !cmd->filepath)
		return (EXIT_FAILURE);
	skip_whitespaces(&line);
	if (*line == '|')
		return (*line);
	else if (!*line)
		return ('\n');
	return (parse_test(sh, line));
}

int	parse_test(t_shell *sh, char *line)
{
	t_cmd	*curr_cmd;
	int		loop_i;

	if (!line || !*line)
		return (EXIT_SUCCESS);
	curr_cmd = add_new_cmd(sh);
	skip_whitespaces(&line);
	loop_i = 0;
	while (*line && *line != '|')
	{
		if (is_set(*line, "<>"))
			curr_cmd->errnum = parse_redir(curr_cmd, &line);
		else
			add_cmd_arg(curr_cmd, get_cl_tok(&line)); //assigns cmd vs cmd_arg
		if (curr_cmd->errnum)
			return (curr_cmd->errnum);
		skip_whitespaces(&line);
		loop_i++;
	}
	if (*line == '|')
		return (check_parse(sh, curr_cmd, ++line));
	return (EXIT_SUCCESS);
}
/**
 *	@brief Parsing logic has changed!
 *
 *		Expansion cannot happen at parse time.
 *
 *		Therefore, parse will only attribute strings to buckets,
 *		and the expansion will happen at execution time.
 *
 *		Expansion functions still need to be written, but not for parsing
 *
 *		Parsing errors need to be detected:
 *			- multiple special characters
 *			- redirection character without an associated target
 *
 *	CHALLENGES:
 *		1.Parse conditions can get quite complex quite quickly. 
 *			Making sure that every outcome is systematically taken care of is a complexity challenge
 *		2.`'`, `"`, `<`, `>`, `|` seem to be my only conditions for breaking parsing so far
 *		3. Managing expansions and parse errors with repeats of `>>>` or `||` for now
 *
 *	PLAN:
 *		1. Parse into strings.
 *		2. Print the parsed string array
 *		3. Assign strings to respective buckets
 *
 *	NOTES:
 *		- tokenize groups (separated by either a whitespace or a CL_SPEC_CH)
 *		- "expand" quotes, as in group them together
 *		- I need to have special parse statuses that tell me where I am in my parsing. 
 *		- Expansion parsing is CRUCIAL for env var parsing
 *
 *	DECISION:
 *		- expansion happens at parse time except for env variables, prepended by c == 26
 *		- everything else will be parsed and expanded as shell variables
 *		- `$` on a single line will be converted to `\26` (SUB)
 *
 */
