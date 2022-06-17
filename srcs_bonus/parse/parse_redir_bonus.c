/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iyahoui- <iyahoui-@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/13 16:58:26 by jgoad             #+#    #+#             */
/*   Updated: 2022/06/16 18:00:57 by iyahoui-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

void	add_infile(t_cmd *cmd, t_infile *new_in)
{
	t_infile	**new_arr;
	int			i;

	new_arr = ft_xalloc((cmd->nb_ins + 2) * sizeof(t_infile *));
	i = 0;
	while (i < cmd->nb_ins)
	{
		new_arr[i] = cmd->ins[i];
		i++;
	}
	new_arr[i++] = new_in;
	new_arr[i] = NULL;
	free(cmd->ins);
	cmd->ins = new_arr;
}

void	add_outfile(t_cmd *cmd, t_outfile *new_out)
{
	t_outfile	**new_arr;
	int			i;

	new_arr = ft_xalloc((cmd->nb_outs + 2) * sizeof(t_outfile *));
	i = 0;
	while (i < cmd->nb_outs)
	{
		new_arr[i] = cmd->outs[i];
		i++;
	}
	new_arr[i++] = new_out;
	new_arr[i] = NULL;
	free(cmd->outs);
	cmd->outs = new_arr;
}

void	parse_in(t_cmd *cmd, char **line, bool is_double)
{
	t_infile	*new_in;

	new_in = ft_xalloc(sizeof(t_infile));
	new_in->fd = -1;
	if (is_double)
		set_cl_tok(&new_in->delim_lst, line);
	else
		set_cl_tok(&new_in->in_lst, line);
	if (is_double)
		parse_heredoc(cmd, new_in);
	add_infile(cmd, new_in);
	cmd->nb_ins++;
}

void	parse_out(t_cmd *cmd, char **line, bool is_double)
{
	t_outfile	*new_out;

	new_out = ft_xalloc(sizeof(t_outfile));
	new_out->fd = -1;
	set_cl_tok(&new_out->out_lst, line);
	new_out->append_mode = is_double;
	add_outfile(cmd, new_out);
	cmd->nb_outs++;
}

int	parse_redir(t_cmd *cmd, char **line)
{
	char		*ptr;
	bool		is_double;

	is_double = false;
	ptr = *line + 1;
	if (*ptr == *(ptr - 1))
	{
		is_double = true;
		ptr++;
	}
	skip_whitespaces(&ptr);
	if (!*ptr)
		return ('\n');
	else if (is_set(*ptr, "<>|"))
		return (*ptr);
	if (**line == '<')
		parse_in(cmd, &ptr, is_double);
	else if (**line == '>')
		parse_out(cmd, &ptr, is_double);
	*line = ptr;
	return (0);
}