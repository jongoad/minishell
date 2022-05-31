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

	// if (!envp || !*envp || !ptr || !*ptr)
	// 	return (NULL);
	token = NULL;
	if (**ptr != '$')
	{
		printf("non_expanded str: %s\n", *ptr);
		tok_len = get_tok_len(*ptr, "$");
		printf("tok_len: %d\n", tok_len);
		token = ft_xalloc((tok_len + 1) * sizeof(char));
		token = ft_strncpy(token, *ptr, tok_len);
		printf("token = %s\n", token);
		*ptr += tok_len;
		return (token);
	}
	*ptr += 1;
	printf("var to expand: %s\n", *ptr);
	tok_len = get_tok_len(*ptr, CL_TOK_LIM);
	printf("tok_len: %d\n", tok_len);
	printf("tok_len: %d\n", tok_len);
	printf("tok_len: %d\n", tok_len);
	var_name = ft_xalloc((tok_len + 1) * sizeof(char));
	printf("after ft_xalloc: %d\n", tok_len);
	var_name = ft_strncpy(token, *ptr, tok_len);
	puts("after ft_strncpy");
	token = expand_env_var(envp, var_name);
	printf("expanded variable = %s\n", token);
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
	printf("expanded = %s\n", expanded);
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
		write(in->fd, buff, len);
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
	int		status;
	pid_t	pid;

	in->infile = ft_strdup("./heredoc");			// Has to be switched to a unique name
	//	Not sure if assigning in_file is necessary
	set_cl_tok(&in->in_lst, &in->infile);			// Not sure if needed or just need condition in lst_to_str
	in->infile -= ft_strlen("./heredoc");			// hacky bullshit to make up for reverse order of operations
	
	in->delim = lst_to_str_no_exp(in->delim_lst);
	printf("delim = %s\n", in->delim);
	in->fd = open(in->infile, O_TRUNC | O_CREAT | O_CLOEXEC | O_RDWR, 0644);
	pid = fork();
	if (pid == 0)
		read_heredoc(cmd, in);
	status = 0;
	waitpid(pid, &status, 0);
	printf("heredoc child exited\n");
	close(in->fd);
	return ;
}
