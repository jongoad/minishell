#include "minishell.h"

/* If a wildcard is found, expand and return an array of strings containing all results */
char **expand_wildcard(char *arg)
{
	t_wildcard	wc;

	split_path_wildcard(&wc, arg);
	wc.ends = malloc(sizeof(int) * 2);
	check_ends(wc.str, wc.ends);									/* Check for end wildcards */
	wc.search = ft_split(wc.str, WILDCARD);								/* Split remaining string using asterisk */
	if (wc.path)													/* If there is a relative path use for getting folder */
		wc.output = read_directory(wc.path);
	else															/* Else open current directory */
		wc.output = read_directory(".");
	wc.output = search_directory(wc.output, wc.search, wc.ends);	/* Run search on directory results, return only matching results. If null returned there are no results */
	if (wc.path)
		add_path_wildcard(wc.output, wc.path);						/* Join path back onto result strings */

	/* Free memory before exit */
	if (wc.path)
		free(wc.path);
	if (wc.str)
		free(wc.str);
	free(wc.ends);
	free_array((void **)wc.search);
	return (wc.output);
}
//Need to adjust the return to be NULL if there is no change FIX
//Change to use 26 instead of *

/* Check for presence of path information */
void	split_path_wildcard(t_wildcard *wc, char *arg)
{
	int	i;

	i = ft_strlen(arg);
	while (i >= 0)
	{
		if (arg[i] == '/')
			break;
		i--;
	}
	if (i != -1)							/* If a slash found */
	{
		wc->str = get_last_token(arg, '/');	/* Copy non-path segment */
		arg[i + 1] = '\0';
		wc->path = ft_strdup(arg);			/* Copy the path segment */
	}
	else
	{
		wc->str = ft_strdup(arg);
		wc->path = NULL;
	}
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

/* Open and read directory, returning an array of file names */
char **read_directory(char *path)
{
	DIR *fd;
	struct dirent *direct;
	char **ret;

	ret = NULL;
	fd = opendir(path);
	if (!fd)
	{
		//error messaging
		/* cat: ../sadsadsd/sadsa*: No such file or directory */
		return (NULL);
	}
	direct = readdir(fd);
	if (!direct)
	{
		closedir(fd);
		//Error messaging
		return (NULL);
	}
	while (direct)
	{
		ret = add_str_array(ret, direct->d_name);
		direct = readdir(fd);
	}
	closedir(fd);
	return (ret);
}

/* Search the directory array using wildcard search array */
char **search_directory(char **direct, char **search, int *ends)
{
	char	**ret;
	int	i;

	ret = NULL;
	i = 0;
	while (direct[i])
	{
		if (is_wildcard_match(direct[i], search, ends))
			ret = add_str_array(ret, direct[i]);
		i++;
	}
	free_array((void **)direct);
	return (ret);
}


/* Check if a string is a wildcard match */
bool	is_wildcard_match(char *direct, char **search, int *ends)
{
	t_wildcard wc;

	wc.start = 0;
	wc.end = count_array((void **)search) - 1;
	wc.tmp = ft_strdup(direct);
	wc.p_tmp = wc.tmp;

	if (!search[0])															/* If there are search tokens (only wildcard chars were entered) */
		return (free_return_bool(wc.tmp, true));
	if (ft_strlen(wc.tmp) < (size_t)get_search_tot(search))					/* If total length of string is not equal to or greater than sum of search strings return false */
		return (free_return_bool(wc.tmp, false));

	if (ends[0] == 0)														/* If no starting wildcard and no match at start return false */
	{
		if (ft_strncmp(wc.tmp, search[wc.start], ft_strlen(search[wc.start])))
			return (free_return_bool(wc.tmp, false));
		wc.tmp = wc.tmp + ft_strlen(search[wc.start]);						/* Shorten string from front */
		wc.start++;
	}
	if (ends[1] == 0)														/* If no ending wildcard and no match end return false */
	{
		if (ft_strncmp(wc.tmp + (ft_strlen(wc.tmp) - ft_strlen(search[wc.end])), search[wc.end], ft_strlen(search[wc.end])))
			return (free_return_bool(wc.p_tmp, false));
		wc.tmp[ft_strlen(wc.tmp) - ft_strlen(search[wc.end])] = '\0';			/* Shorten string from back */
		wc.end--;
	}
	while (wc.start <= wc.end)												/* Handle middle wildcard matches */
	{
		wc.ret = ft_strnstr(wc.tmp, search[wc.start], ft_strlen(wc.tmp));
		if (!wc.ret)
			return (free_return_bool(wc.p_tmp, false));
		if (wc.tmp)
			free(wc.tmp);
		wc.tmp = wc.ret;
		wc.tmp = wc.tmp + ft_strlen(search[wc.start]);						/* Shorten string */
		wc.start++;
	}
	return (free_return_bool(wc.p_tmp, true));
}