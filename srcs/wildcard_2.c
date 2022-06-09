#include "minishell.h"

/* Open and read directory, returning an array of file names */
char **read_directory(char *path)
{
	DIR *fd;
	struct dirent *direct;
	char **ret;

	ret = NULL;
	fd = opendir(path);
	if (!fd)
		return (NULL);
	direct = readdir(fd);
	if (!direct)
	{
		closedir(fd);
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
	if (!is_wildcard_match_ends(&wc, search, ends))								/* Check end cases */
		return (free_return_bool(wc.tmp, false));
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

/* Check end cases for wildcard match */
bool	is_wildcard_match_ends(t_wildcard *wc, char **search, int *ends)
{
	if (ends[0] == 0)															/* If no starting wildcard and no match at start return false */
	{
		if (ft_strncmp(wc->tmp, search[wc->start], ft_strlen(search[wc->start])))
			return (false);
		wc->tmp = wc->tmp + ft_strlen(search[wc->start]);						/* Shorten string from front */
		wc->start++;
	}
	if (ends[1] == 0)															/* If no ending wildcard and no match end return false */
	{
		if (ft_strncmp(wc->tmp + (ft_strlen(wc->tmp) - ft_strlen(search[wc->end])),
			search[wc->end], ft_strlen(search[wc->end])))
			return (false);
		wc->tmp[ft_strlen(wc->tmp) - ft_strlen(search[wc->end])] = '\0';		/* Shorten string from back */
		wc->end--;
	}
	return (true);
}
