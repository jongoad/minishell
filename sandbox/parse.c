#include "parse_test.h"

void	skip_whitespaces(char **line)
{
	if (!line || !*line)
		return ;
	while (**line && is_set(**line, WHITESPACES))
		*line += 1;
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
 *		- tokenize groups (separated by either a whitespace or a PARSE_SPEC_CH)
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
char	*get_cl_tok(char **line)
{
	char	*word;
	int		word_len;

	if (!line || !*line)
		return (NULL);
	word_len = 0;
	while (*line[word_len] && !is_set(*line[word_len], WHITESPACES))
	{
		if (*line[word_len] == '|')
			break ;
		word_len++;
	}
	if (!word_len)
		return (NULL);
	word = ft_xalloc(word_len * sizeof(char));
	word_len = 0;
	while (*line[word_len] && !is_set(**line, WHITESPACES))
	{
		word[word_len] = **line;
		word_len++;
		*line += 1;
	}
	return (word);
}

int	parse_test(t_shell *sh, char *rem_line)
{
	t_cmd	*curr_cmd;
	char	*line;

	if (!*rem_line)
		return (EXIT_SUCCESS);
	curr_cmd = add_cmd(sh);
	line = rem_line;
	skip_whitespaces(&line);
	if (*line && is_set(*line, PARSE_SPEC_CH))
		get_cl(curr_cmd, &line);
	while (*line && !is_set(*line, "|"))
	{
		skip_whitespaces(&line);
		if (*line && is_set(*line, PARSE_SPEC_CH))
			parse_special_chars(curr_cmd, &line);
		if (!*line || is_set(*line, "|"))
			return (EXIT_SUCCESS);
		add_cmd_arg(curr_cmd, get_cl_tok(&line));
	}
	return (EXIT_SUCCESS);
}
