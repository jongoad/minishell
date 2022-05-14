#include "minishell.h"

void	add_token(t_arglst **lst, char **line, char *delimiter, bool is_env_var)
{
	char	*token;
	int		tok_len;

	tok_len = get_tok_len(*line, delimiter);
	token = ft_xalloc((tok_len + 1) * sizeof(char));
	token = ft_strncpy(token, *line, tok_len);
	ms_lstadd(lst, ms_lstnew(token, is_env_var));
	*line += tok_len;
	return ;
}

void	parse_dquotes(t_arglst **lst, char **line)
{
	int		tok_len;

	tok_len = get_tok_len(*line + 1, "\"");
	if (!(*line)[tok_len])
		return (add_token(lst, line, CL_TOK_LIM, false));
	*line += 1;
	tok_len = get_tok_len(*line, "\"$");
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

	tok_len = get_tok_len(*line + 1, "\'");
	if (!(*line)[tok_len])
		return (add_token(lst, line, CL_TOK_LIM, false));
	*line += 1;
	add_token(lst, line, "\'", false);
	*line += 1;
}

void	set_cl_tok(t_arglst **lst, char **line)
{
	if (!line || !*line)
		return ;
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
		return (set_cl_tok(lst, line));
}
