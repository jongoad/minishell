#include "parse_test.h"

void	skip_whitespaces(char **line)
{
	if (!line || !*line)
		return ;
	while (**line && is_set(**line, WHITESPACES))
		*line += 1;
}

char	*get_word(char **line)
{
	char	*word;
	int		word_len;

	if (!line || !*line)
		return (NULL);
	word_len = 0;
	while (*line[word_len] && !is_set(*line[word_len], WHITESPACES))
	{
		// Account for '$' expansion
		
		// Account for squotes and dquotes

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

int	parse_test(t_shell *sh)
{
	t_cmd	*curr_cmd;
	char	*line;

	curr_cmd = add_cmd(sh);
	line = sh->line;
	skip_whitespaces(&line);
	if (*line && is_set(*line, PARSE_SPEC_CH))
		parse_special_chars(curr_cmd, &line);
	curr_cmd->filepath = get_word(&line);
	if (!curr_cmd->filepath)
	{
		curr_cmd->errname = ft_strncpy(curr_cmd->errname, &line, 1);
		curr_cmd->errnum = 12; // E_PARSE
		return (curr_cmd->errnum);
	}
	while (*line)
	{
		skip_whitespaces(&line);
		if (*line && is_set(*line, PARSE_SPEC_CH))
			parse_special_chars(curr_cmd, &line);
		if (!*line || is_set(*line, "|"))
			return (EXIT_SUCCESS);
		add_cmd_arg(curr_cmd, get_word(&line));
	}
	return (EXIT_SUCCESS);
}
