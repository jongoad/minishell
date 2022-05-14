#include "minishell.h"

/**
 * @brief Get the tok len object
 * 
 * @param token 
 * @param delimiters Set of chars that represent the end of token
 * @return int 		 tok_len until the first delimiter encountered
 * 					 -1 if an arg is NULL
 */
int get_tok_len(char *token, char *delimiters)
{
	int  tok_len;

	if (!token)
		return (-1);
	tok_len = 0;
	while (token[tok_len] && !is_set(token[tok_len], delimiters))
		tok_len++;
	return (tok_len);
}

void	add_token(t_arglst **lst, char **line, char *delimiter, bool is_env_var)
{
	char	*token;
	int		tok_len;

	tok_len = get_tok_len(*line, delimiter);
	token = ft_xalloc((tok_len + 1) * sizeof(char));
	token = ft_strncpy(token, *line, tok_len);
	ms_lstadd(lst, ms_lstnew(token, false));
	*line += tok_len;
	return ;
}

void	parse_dquotes(t_arglst **lst, char **line)
{
	int		tok_len;

	tok_len = get_tok_len(*line + 1, "\"");
	if (!(*line)[tok_len])
		return (add_token(lst, line, CL_TOK_LIM, false));
	tok_len = get_tok_len(*line + 1, "\"$");
	while (tok_len)
	{
		if ((*line)[tok_len] == '$')
		{
			add_token(lst, line, "$", true);
			tok_len = get_tok_len(*line + 1, "\"$");
		}
		else
		{
			add_token(lst, line, "\"", false);
			tok_len = 0;
		}
	}
}

void	parse_squotes(t_arglst **lst, char **line)
{
	int		tok_len;

	tok_len = get_tok_len(*line + 1, "\"");
	if (!(*line)[tok_len])
		return (add_token(lst, line, CL_TOK_LIM, false));
	*line += 1;
	add_token(lst, line, "\'", false);
	*line += 1;
}

t_arglst	*get_cl_tok(t_arglst **lst, char **line)
{
	t_arglst	*new_arg;
	char		*token;
	int			tok_len;

	if (!line || !*line)
		return (NULL);
	if (**line == '\"')
		parse_dquotes(lst, line);
	else if (**line == '\'')
		parse_squotes(lst, line);
	else if (**line == '$')
	{
		*line += 1;
		add_token(lst, line, CL_ENV_VAR_LIM, true);
	}
	else
		add_token(lst, line, CL_TOK_LIM, false);
	if (is_set(**line, CL_SPEC_CH))
		return (get_cl_tok(lst, line));
	return (*lst);
}
