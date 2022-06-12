#include "minishell.h"

int	parse(t_shell *sh, char *line)
{
	t_cmd	*curr_cmd;

	if (!line || !*line)
		return (EXIT_SUCCESS);
	// printf("ms entered parse with nb_cmds == %d\n", sh->nb_cmds);
	// printf("sh->cmds == %p\n", sh->cmds);
	curr_cmd = add_new_cmd(sh);
	// printf("after add_new_cmd, nb_cmds == %d\n", sh->nb_cmds);
	skip_whitespaces(&line);
	while (*line && *line != '|')
	{
		if (is_set(*line, "<>"))
			curr_cmd->errnum = parse_redir(curr_cmd, &line);
		else
			add_cmd_arg(curr_cmd, &line); // all cmd_args are parsed in the same linked list
		if (curr_cmd->errnum)
			return (curr_cmd->errnum);
		skip_whitespaces(&line);
	}
	if (*line)
		return (check_parse(sh, curr_cmd, ++line));
	return (0);
}

int	check_parse(t_shell *sh, t_cmd *cmd, char *line)
{
	if (!cmd->ins && !cmd->outs && !cmd->args_lst)
		return (*(line - 1));
	skip_whitespaces(&line);
	if (*line == '|')
		return (*line);
	else if (!*line)
		return ('\n');
	return (parse(sh, line));
}
