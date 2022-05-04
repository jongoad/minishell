#include "parse_test.h"

t_cmd	*get_new_cmd(void)
{
	t_cmd	*cmd;

	cmd = ft_xalloc(sizeof(t_cmd));
	cmd->args = ft_xalloc(sizeof(char *));
	cmd->io.infiles = ft_xalloc(sizeof(char *));
	cmd->io.outfiles = ft_xalloc(sizeof(char *));
	return (cmd);
}

t_cmd	*add_cmd(t_shell *sh)
{
	t_cmd	**new_arr;
	t_cmd	*new_cmd;
	int		i;

	sh->nb_cmds++;
	new_arr = ft_xalloc((sh->nb_cmds + 1) * sizeof(t_cmd *));
	new_cmd = get_new_cmd();
	i = 0;
	while (i < sh->nb_cmds - 1)
	{
		new_arr[i] = sh->cmds[i];
		i++;
	}
	new_arr[i++] = new_cmd;
	new_arr[i] = NULL;
	free(sh->cmds);
	sh->cmds = new_arr;
	return (new_cmd);
}

char	*get_word(char **line)
{
	char	*word;
	int		i;

	if (!line || !*line)
		return (NULL);
	i = 0;
	while (*line[i] && !is_set(*line[i], WHITESPACES))
	{
		
	}
}

int	parse_test(t_shell *sh)
{
	t_cmd	*curr_cmd;
	char	*line;

	curr_cmd = add_cmd(sh);
	line = sh->line;
	skip_whitespaces(&line);
	parse_special_chars(curr_cmd, &line);
	curr_cmd->filepath = get_word(&line);

	while (line[i])
	{
		parse_redirs(&line[i]);
		
	}
}
