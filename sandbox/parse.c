#include "parse_test.h"

void	skip_whitespaces(char **line)
{
	if (!line || !*line)
		return ;
	while (**line && is_set(**line, WHITESPACES))
		*line += 1;
}

char	*get_cl_tok(char **line)
{
	char	*token;
	int		tok_len;

	if (!line || !*line)
		return (NULL);
	tok_len = 0;
	while (*line[tok_len] && !is_set(*line[tok_len], WHITESPACES))
	{
		if (is_set(*line[tok_len], ""))
			break ;
		tok_len++;
	}
	if (!tok_len)
		return (NULL);
	token = ft_xalloc((tok_len + 1) * sizeof(char));
	tok_len = 0;
	while (**line && !is_set(**line, WHITESPACES))
	{
		token[tok_len] = **line;
		tok_len++;
		*line += 1;
	}
	token[tok_len] = 0;
	return (token);
}

int	parse_redir(t_cmd *cmd, char *line)
{
	//	Handle in/out

	//	Handle error '>>>' etc

	//	Assign file

	//	get_cl_tok
	return (0);
}

int	check_parse(t_shell *sh, t_cmd *cmd, char *line)
{
	printf("entered check_parse\n");
	printf("line : \"%s\"\n", line);
	if (!(cmd->ins && cmd->ins->infile)
		&& !(cmd->outs && cmd->outs->outfile)
		&& !(cmd->filepath))
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

	printf("entered parse_test\n");
	if (!line || !*line)
		return (EXIT_SUCCESS);
	curr_cmd = add_cmd(sh);
	skip_whitespaces(&line);
	loop_i = 0;
	while (*line && *line != '|')
	{
		// if (is_set(*line, "<>"))
		// {
		// 	if (parse_redir(curr_cmd, &line) == EXIT_FAILURE)
		// 		return (EXIT_FAILURE);
		// }
		// else
		printf("loop #%i\n", loop_i);
		printf("line : \"%s\"\n", line);
			add_cmd_arg(curr_cmd, get_cl_tok(&line)); //assigns cmd vs cmd_arg
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
