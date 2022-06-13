#include "minishell.h"

void	add_token(t_arglst **lst, char **line, char *delim, bool is_env_var)
{
	char	*token;
	int		tok_len;

	tok_len = get_tok_len(*line, delim);
	token = ft_xalloc((tok_len + 1) * sizeof(char));
	token = ft_strncpy(token, *line, tok_len);
	ms_lstadd(lst, ms_lstnew(token, is_env_var));
	*line += tok_len;
	return ;
}

void	add_token_by_set(t_arglst **lst, char **line, char *set, bool is_env_var)
{
	char	*token;
	int		tok_len;

	tok_len = get_tok_len_set(*line, set);
	token = ft_xalloc((tok_len + 1) * sizeof(char));
	token = ft_strncpy(token, *line, tok_len);
	ms_lstadd(lst, ms_lstnew(token, is_env_var));
	*line += tok_len;
	return ;
}

void	add_token_by_len(t_arglst **lst, char **line, int tok_len, bool is_env_var)
{
	char	*token;

	token = ft_xalloc((tok_len + 1) * sizeof(char));
	token = ft_strncpy(token, *line, tok_len);
	ms_lstadd(lst, ms_lstnew(token, is_env_var));
	*line += tok_len;
	return ;
}

/**!
 * 	@brief	If unclosed, treats the token as without quotes
 * 			Otherwise, tokenizes content of dquotes into:
 * 			- pure strings
 * 			- env_vars
 */
void	parse_dquotes(t_arglst **lst, char **line)
{
	int		tok_len;

	tok_len = get_tok_len(*line + 1, "\"");
	if (!(*line)[tok_len + 1])
		return (add_token(lst, line, CL_DQU_LIM, false));
	*line += 1;
	while (**line)	//bogus condition to reduce chances of segfault
	{
		tok_len = get_tok_len(*line, "\"$");
		if ((*line)[tok_len] == '\"')
		{
			add_token(lst, line, "\"", false);
			*line += 1;
			return ;
		}
		else if (**line == '$')
			parse_env_var(lst, line);
		else
			add_token(lst, line, CL_DQU_NOSPEC, false);
	}
}

void	parse_env_var(t_arglst **lst, char **line)
{
	*line += 1;
	if (**line && (ft_isalnum(**line) 
			|| (**line == '?' && (ft_isspace(*(*line + 1)) || !*(*line + 1)))))
	{
		if (**line == '?')
			add_token_by_len(lst, line, 1, true);
		else
			add_token_by_set(lst, line, ENV_VAR_CHARS, true);
	}
	else
	{
		*line -= 1;
		add_token(lst, line, CL_VAR_LIM, false);
	}
}

/**!
 * 	@brief	Returns the entirety of the string in squotes
 */
void	parse_squotes(t_arglst **lst, char **line)
{
	int		tok_len;

	tok_len = get_tok_len(*line + 1, "\'");
	if (!(*line)[tok_len + 1])
		return (add_token(lst, line, CL_SQU_LIM, false));
	*line += 1;
	add_token(lst, line, "\'", false);
	*line += 1;
}

/**
 * @brief 	Set the cl token as a linked list of strings
 * 			Advances the `line' pointer to the end of the read token
 * 
 * @param lst	The address of the linked list to which the token will be added
 * @param line 	The pointer to the beginning of the token
 */
void	set_cl_tok(t_arglst **lst, char **line)
{
	if (!line || !*line)
		return ;
	if (**line == '\"')
		parse_dquotes(lst, line);
	else if (**line == '\'')
		parse_squotes(lst, line);
	else if (**line == '$')
		parse_env_var(lst, line);
	else
		add_token(lst, line, CL_TOK_LIM, false);
	if (is_set(**line, CL_SAME_TOK) || ft_isalnum(**line))
		return (add_cl_tok(lst, line));
}

// Adds tokens that belong together by setting the arglst->is_joined flag
void	add_cl_tok(t_arglst **lst, char **line)
{
	if (!line || !*line)
		return ;
	if (**line == '\"')
		parse_dquotes(lst, line);
	else if (**line == '\'')
		parse_squotes(lst, line);
	else if (**line == '$')
		parse_env_var(lst, line);
	else
		add_token(lst, line, CL_TOK_LIM, false);
	ms_lstlast(*lst)->is_joined = true;
	if (is_set(**line, CL_SAME_TOK) || ft_isalnum(**line) || **line == '=')
		return (add_cl_tok(lst, line));
}

void	parse_wildcard(t_arglst **lst, char **line)
{
	add_token_by_len(lst, line, 1, true);
	while (**line && **line == '*')
		*line += 1;
}

/**
 * @brief 	Set the cl token as a linked list of strings
 * 			Advances the `line' pointer to the end of the read token
 * 
 * @param lst	The address of the linked list to which the token will be added
 * @param line 	The pointer to the beginning of the token
 */
void	set_cl_tok_bonus(t_arglst **lst, char **line)
{
	if (!line || !*line)
		return ;
	if (**line == '\"')
		parse_dquotes(lst, line);
	else if (**line == '\'')
		parse_squotes(lst, line);
	else if (**line == '$')
		parse_env_var(lst, line);
	else if (**line == '*')
		parse_wildcard(lst, line);
	else
		add_token(lst, line, CL_TOK_LIM_BONUS, false);
	if (is_set(**line, CL_SAME_TOK_BONUS) || ft_isalnum(**line) || **line == '=')
		return (add_cl_tok_bonus(lst, line));
}

// Adds tokens that belong together by setting the arglst->is_joined flag
void	add_cl_tok_bonus(t_arglst **lst, char **line)
{
	if (!line || !*line)
		return ;
	if (**line == '\"')
		parse_dquotes(lst, line);
	else if (**line == '\'')
		parse_squotes(lst, line);
	else if (**line == '$')
		parse_env_var(lst, line);
	else if (**line == '*')
		parse_wildcard(lst, line);
	else
		add_token(lst, line, CL_TOK_LIM_BONUS, false);
	ms_lstlast(*lst)->is_joined = true;
	if (is_set(**line, CL_SAME_TOK_BONUS) || ft_isalnum(**line) || **line == '=')
		return (add_cl_tok_bonus(lst, line));
}
