/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iyahoui- <iyahoui-@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/13 16:57:54 by jgoad             #+#    #+#             */
/*   Updated: 2022/06/21 14:59:05 by iyahoui-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

void	close_heredoc(int signum)
{
	t_shell	*sh;

	sh = get_data();
	cleanup(sh);
	(void)signum;
	exit(0);
}

char	*expand_heredoc_tok(char **envp, char **ptr)
{
	char	*var_name;
	char	*token;
	int		tok_len;
	int		i;

	token = NULL;
	if (**ptr != '$')
	{
		tok_len = get_tok_len(*ptr, "$");
		token = ft_xalloc((tok_len + 1) * sizeof(char));
		token = ft_strncpy(token, *ptr, tok_len);
		return (*ptr += tok_len, token);
	}
	*ptr += 1;
	tok_len = 0;
	while (ft_isalnum((*ptr)[tok_len]) || (*ptr)[tok_len] == '_')
		tok_len++;
	var_name = ft_xalloc((tok_len + 1) * sizeof(char));
	i = -1;
	while (++i < tok_len)
		var_name[i] = (*ptr)[i];
	var_name[i] = 0;
	token = expand_env_var(envp, var_name);
	free(var_name);
	return (*ptr += tok_len, token);
}

void	expand_heredoc(t_cmd *cmd, t_infile *in, char *heredoc)
{
	char	*expanded;
	char	*token;
	char	*ptr;

	expanded = NULL;
	ptr = heredoc;
	while (ptr && *ptr)
	{
		token = expand_heredoc_tok(cmd->envp, &ptr);
		expanded = ft_strjoin_free(expanded, token);
		free(token);
	}
	write(in->fd, expanded, ft_strlen(expanded));
	free(expanded);
}

static void	read_heredoc(t_cmd *cmd, t_infile *in)
{
	t_shell	*sh;
	char	*heredoc;
	char	*buff;
	int		len;

	(void)cmd;
	sh = get_data();
	signal(SIGINT, close_heredoc);
	heredoc = NULL;
	while (1)
	{
		putstr_fd("> ", STDOUT_FILENO);
		buff = get_next_line(STDIN_FILENO);
		len = ft_strlen(buff);
		if (!buff || (!ft_strncmp(buff, in->delim, len - 1) && len > 1))
			break ;
		heredoc = ft_strjoin_free(heredoc, buff);
		free (buff);
	}
	free (buff);
	expand_heredoc(cmd, in, heredoc);
	free (heredoc);
	close(in->fd);
	cleanup(sh);
	exit (0);
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
		read_heredoc(cmd, in);
	status = 0;
	waitpid(pid, &status, 0);
	close(in->fd);
	signal(SIGINT, signal_handler);
}
