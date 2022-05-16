#include "minishell.h"

char	*expand_heredoc_tok(char **envp, char *ptr)
{
	char	*var_name;
	char	*token;
	int		tok_len;

	// if (!envp || !*envp || !ptr || !*ptr)
	// 	return (NULL);
	token = NULL;
	if (*ptr != '$')
	{
		printf("non_expanded str: %s\n", ptr);
		tok_len = get_tok_len(ptr, "$");
		printf("tok_len: %d\n", tok_len);
		token = ft_xalloc((tok_len + 1) * sizeof(char));
		token = ft_strncpy(token, ptr, tok_len);
		printf("token = %s\n", token);
		return (token);
	}
	ptr += 1;
	printf("var to expand: %s\n", ptr);
	tok_len = get_tok_len(ptr, CL_TOK_LIM);
	printf("tok_len: %d\n", tok_len);
	var_name = ft_xalloc((tok_len + 1) * sizeof(char));
	var_name = ft_strncpy(token, ptr, tok_len);
	token = expand_env_var(envp, var_name);
	printf("token = %s\n", token);
	free(var_name);
	// *ptr += tok_len;
	return (token);
}

void	expand_heredoc(t_cmd *cmd, t_infile *in, char *heredoc)
{
	char	*expanded;
	char	*token;
	char	*ptr;
	int		tok_len;

	expanded = NULL;
	ptr = heredoc;
	while (ptr && *ptr)
	{
		token = expand_heredoc_tok(cmd->envp, ptr);
		tok_len = ft_strlen(token);
		expanded = ft_strjoin_free(expanded, token);
		free(token);
		ptr += tok_len;
	}
	write(in->fd, expanded, ft_strlen(expanded));
	printf("expanded = %s\n", expanded);
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
		heredoc = ft_strjoin_free(heredoc, buff);
		free (buff);
	}
	free (buff);

	//	Right now, expand_heredoc does not seem to work properly
	// expand_heredoc(cmd, in, heredoc);

	write(in->fd, heredoc, ft_strlen(heredoc));
	free (heredoc);
	close(in->fd);
	exit (0);
}

void	parse_heredoc(t_cmd *cmd, t_infile *in)
{
	int		status;
	pid_t	pid;

	//	Not sure if assigning in_file is necessary
	in->infile = ft_strdup("./heredoc");			// To switch to a proper name
	set_cl_tok(&in->in_lst, &in->infile);
	in->infile -= ft_strlen("./heredoc");			// hacky bullshit
	
	in->delim = lst_to_str_no_exp(in->delim_lst);	//except without env_var expansion
	printf("delim = %s\n", in->delim);
	in->fd = open(in->infile, O_TRUNC | O_CREAT | O_CLOEXEC | O_RDWR, 0644);
	pid = fork();
	if (pid == 0)
		read_heredoc(cmd, in);
	status = 0;
	waitpid(pid, &status, 0);
	close(in->fd);
	return ;
}
