#include "minishell.h"

/* Wildcard integration function */
void	check_wildcard(t_cmd *cmd)
{
	char	**result;
	char	**output;
	int		i;

	result = NULL;
	output = NULL;
	i = 0;
	if (check_no_wildcard(cmd))
		return ;
	while (i < cmd->nb_args)
	{
		result = expand_wildcard(cmd->args[i]);
		if (!result)														/* If no result copy string to output array, adding back in the asterisk if it existed */
			output = add_str_array(output, replace_wildcard(cmd->args[i]));
		else																/* If there is a result copy result to output array */
			output = join_array_array(output, result);									
		i++;
	}
	free_array((void **)cmd->args);											/* Free args array and then assign new array to pointer */
	cmd->args = output;
	free(cmd->exe);															/* Free exe and set to first element of new args array */
	cmd->exe = ft_strdup(output[0]);
	cmd->nb_args = count_array((void **)output);							/* Update new args number */
}

/* Initialize wildcard data */
void	init_wildcard(t_wildcard *wc, char *arg)
{
	split_path_wildcard(wc, arg);
	wc->ends = malloc(sizeof(int) * 2);
	check_ends(wc->str, wc->ends);									/* Check for end wildcards */
	wc->search = ft_split(wc->str, WILDCARD);						/* Split remaining string using asterisk */
}

/* If a wildcard is found, expand and return an array of strings containing all results */
char	**expand_wildcard(char *arg)
{
	t_wildcard	wc;

	if (!is_set(WILDCARD, arg))	/* Check if any wildcards exist, if not return NULL */
		return (NULL);
	init_wildcard(&wc, arg);
	if (wc.path)													/* If there is a relative path use for getting folder */
		wc.output = read_directory(wc.path);
	else															/* Else open current directory */
		wc.output = read_directory(".");
	if (!wc.output)													/* Handle directory error */
		return (wildcard_error_return((&wc)));
	wc.output = search_directory(wc.output, wc.search, wc.ends);	/* Run search on directory results, return only matching results. If null returned there are no results */
	if (!wc.output)													/* Handle no search results */
		return (wildcard_error_return((&wc)));
	if (wc.path)													/* Join path back onto result strings */
	{
		add_path_wildcard(wc.output, wc.path);
		free(wc.path);
	}
	if (wc.str)
		free(wc.str);
	free(wc.ends);
	free_array((void **)wc.search);
	return (wc.output);
}

/* Check for presence of path information */
void	split_path_wildcard(t_wildcard *wc, char *arg)
{
	char	*tmp;
	int	i;

	tmp = ft_strdup(arg);
	i = ft_strlen(arg);
	while (i >= 0)
	{
		if (tmp[i] == '/')
			break;
		i--;
	}
	if (i != -1)							/* If a slash found */
	{
		wc->str = get_last_token(tmp, '/');	/* Copy non-path segment */
		tmp[i + 1] = '\0';
		wc->path = ft_strdup(tmp);			/* Copy the path segment */
	}
	else
	{
		wc->str = ft_strdup(tmp);
		wc->path = NULL;
	}
	if (tmp)
		free(tmp);
}

/* Append path back on to result strings */
void	add_path_wildcard(char **results, char *path)
{
	int	i;

	i = 0;
	while (results && results[i])
	{
		results[i] = ft_strjoin_free_rev(path, results[i]);
		i++;
	}
}



/* Check if there are no wildcards in any of the args */
bool	check_no_wildcard(t_cmd *cmd)
{
	int	i;

	i = 0;
	while (i < cmd->nb_args)
	{
		if (is_set(WILDCARD, cmd->args[i]))
			return (false);
		i++;
	}
	return (true);
}
