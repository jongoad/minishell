/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iyahoui- <iyahoui-@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/13 16:58:05 by jgoad             #+#    #+#             */
/*   Updated: 2022/06/13 16:58:11 by iyahoui-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* All command line tokens are parsed through parse_redir or add_cmd_arg */
int	parse(t_shell *sh, char *line)
{
	t_cmd	*curr_cmd;

	if (!line || !*line)
		return (EXIT_SUCCESS);
	curr_cmd = add_new_cmd(sh);
	skip_whitespaces(&line);
	while (*line && *line != '|')
	{
		if (is_set(*line, "<>"))
			curr_cmd->errnum = parse_redir(curr_cmd, &line);
		else
			add_cmd_arg(curr_cmd, &line);
		if (curr_cmd->errnum)
			return (curr_cmd->errnum);
		skip_whitespaces(&line);
	}
	if (*line)
		return (check_parse(sh, curr_cmd, ++line));
	return (0);
}

/* Verifies that the command was valid */
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
