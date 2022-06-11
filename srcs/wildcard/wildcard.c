/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoad <jgoad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/09 14:51:21 by jgoad             #+#    #+#             */
/*   Updated: 2022/06/09 14:54:42 by jgoad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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