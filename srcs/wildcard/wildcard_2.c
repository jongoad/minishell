/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoad <jgoad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/09 14:53:42 by jgoad             #+#    #+#             */
/*   Updated: 2022/06/14 14:48:55 by jgoad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Open and read directory, returning an array of file names */
char	**read_directory(t_wildcard *wc, char *path)
{
	DIR				*fd;
	struct dirent	*direct;
	char			**ret;

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
		if (wc->ret_dot)
			ret = add_str_array(ret, direct->d_name);
		else if (!wc->ret_dot && direct->d_name[0] != '.')
			ret = add_str_array(ret, direct->d_name);
		direct = readdir(fd);
	}
	closedir(fd);
	return (ret);
}

/* Search the directory array using wildcard search array */
char	**search_directory(char **direct, char **search, int *ends)
{
	char	**ret;
	int		i;

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

/* Check for presence of path information */
void	split_path_wildcard(t_wildcard *wc, char *arg)
{
	char	*tmp;
	int		i;

	tmp = ft_strdup(arg);
	i = ft_strlen(arg);
	while (i >= 0)
	{
		if (tmp[i] == '/')
			break ;
		i--;
	}
	if (i != -1)
	{
		wc->str = get_last_token(tmp, '/');
		tmp[i + 1] = '\0';
		wc->path = ft_strdup(tmp);
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
