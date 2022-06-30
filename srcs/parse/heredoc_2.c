/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoad <jgoad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/30 15:08:05 by jgoad             #+#    #+#             */
/*   Updated: 2022/06/30 15:08:31 by jgoad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	parse_heredoc(t_cmd *cmd, t_infile *in)
{
	int		status;
	pid_t	pid;

	in->infile = get_heredoc_filename();
	in->in_lst = ms_lstnew(ft_strdup(in->infile), false);
	in->delim = lst_to_str_no_exp(in->delim_lst);
	in->fd = open(in->infile, O_TRUNC | O_CREAT | O_CLOEXEC | O_RDWR, 0644);
	pid = fork();
	if (pid == 0)
		read_heredoc(cmd, in);
	status = 0;
	waitpid(pid, &status, 0);
	close(in->fd);
	signal(SIGINT, signal_handler);
}
