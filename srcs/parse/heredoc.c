/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iyahoui- <iyahoui-@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/13 16:57:54 by jgoad             #+#    #+#             */
/*   Updated: 2022/08/05 17:39:00 by iyahoui-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	read_heredoc(t_cmd *cmd, t_infile *in)
{
	t_shell	*sh;
	char	*heredoc;
	char	*buff;
	size_t	len;

	sh = get_data();
	heredoc = NULL;
	while (1)
	{
		putstr_fd("> ", STDOUT_FILENO);
		buff = get_next_line(STDIN_FILENO);
		len = ft_strlen(buff);
		if (!buff || (!ft_strncmp(buff, in->delim, len - 1)
				&& len - 1 == ft_strlen(in->delim)))
			break ;
		heredoc = ft_strjoin_free(heredoc, buff);
		free (buff);
	}
	free(buff);
	close(STDIN_FILENO);
	get_next_line(STDIN_FILENO);
	expand_heredoc(cmd, in, heredoc);
	free(heredoc);
	cleanup(sh, false);
	exit(0);
}

char	*get_heredoc_filename(void)
{
	char	*suffix;
	char	*filename;
	uint	n;

	n = 0;
	suffix = ft_itoa(n++);
	filename = ft_strjoin(HEREDOC_PATH, suffix);
	while (access(filename, F_OK) == 0)
	{
		free(suffix);
		free(filename);
		suffix = ft_itoa(n++);
		filename = ft_strjoin(HEREDOC_PATH, suffix);
	}
	free(suffix);
	return (filename);
}

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
	{
		signal(SIGINT, close_heredoc);
		read_heredoc(cmd, in);
	}
	status = 0;
	waitpid(pid, &status, 0);
	close(in->fd);
	signal(SIGINT, signal_handler);
}
