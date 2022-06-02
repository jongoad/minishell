#include "minishell.h"

/* Found the issue:
 *	- expand heredoc should work recursively, or at least the entirety of the tokens parsed
 *	
 *	
 */
char	*expand_heredoc_tok(char **envp, char **ptr)
{
	char	*var_name;
	char	*token;
	int		tok_len;
	int		i;

	token = NULL;
	if (**ptr != '$')
	{
		// printf("non_expanded str: %s\n", *ptr);
		tok_len = get_tok_len(*ptr, "$");
		// printf("tok_len: %d\n", tok_len);
		token = ft_xalloc((tok_len + 1) * sizeof(char));
		token = ft_strncpy(token, *ptr, tok_len);
		// printf("token = %s\n", token);
		*ptr += tok_len;
		return (token);
	}
	*ptr += 1;
	// printf("str to expand: %s\n", *ptr);
	tok_len = 0;
	while (ft_isalnum((*ptr)[tok_len]) || (*ptr)[tok_len] == '_')
		tok_len++;
	// printf("tok_len: %d\n", tok_len);
	var_name = ft_xalloc((tok_len + 1) * sizeof(char));
	// var_name = ft_strncpy(token, *ptr, tok_len);
	i = -1;
	while (++i < tok_len)
		var_name[i] = (*ptr)[i];
	var_name[i] = 0;
	token = expand_env_var(envp, var_name);
	// printf("expanded variable = %s\n", token);
	free(var_name);
	*ptr += tok_len;
	return (token);
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
	// printf("expanded = %s\n", expanded);
	write(in->fd, expanded, ft_strlen(expanded));
	free(expanded);
}

static void	read_heredoc(t_cmd *cmd, t_infile *in)
{
	char	*heredoc;
	char	*buff;
	int		len;

	(void)cmd;
	heredoc = NULL;
	while (1)
	{
		putstr_fd("> ", STDOUT_FILENO);
		buff = get_next_line(STDIN_FILENO);
		len = ft_strlen(buff);
		if (!buff || (!ft_strncmp(buff, in->delim, len - 1) && len > 1))
			break ;
		// write(in->fd, buff, len);
		heredoc = ft_strjoin_free(heredoc, buff);
		free (buff);
	}
	free (buff);

	//	Right now, expand_heredoc does not seem to work properly

	// write(in->fd, heredoc, ft_strlen(heredoc));
	expand_heredoc(cmd, in, heredoc);

	free (heredoc);
	close(in->fd);
	exit (0);
}

void	parse_heredoc(t_cmd *cmd, t_infile *in)
{
	char	*suffix;
	uint	n;
	int		status;
	pid_t	pid;

	// chdir("/tmp");
	n = 0;
	suffix = ft_itoa(n++);
	// printf("suffix = %s\n", suffix);
	in->infile = ft_strjoin(HEREDOC_PATH, suffix);
	// printf("in->infile = %s\n", in->infile);
	// printf("access(in->infile, F_OK) =  %d\n", access(in->infile, F_OK));
	while (access(in->infile, F_OK) == 0)
	{
		free(suffix);
		free(in->infile);
		suffix = ft_itoa(n++);
		// printf("suffix = %s\n", suffix);
		in->infile = ft_strjoin(HEREDOC_PATH, suffix);
		// printf("in->infile = %s\n", in->infile);
		// printf("access(in->infile, F_OK) =  %d\n", access(in->infile, F_OK));
	}
	free(suffix);
	in->in_lst = ms_lstnew(ft_strdup(in->infile), false);
	// printf("heredoc name = %s\n", in->in_lst->str);
	
	in->delim = lst_to_str_no_exp(in->delim_lst);
	// printf("delim = %s\n", in->delim);
	in->fd = open(in->infile, O_TRUNC | O_CREAT | O_CLOEXEC | O_RDWR, 0644);
	pid = fork();
	if (pid == 0)
		read_heredoc(cmd, in);
	status = 0;
	waitpid(pid, &status, 0);
	close(in->fd);
	return ;
}
