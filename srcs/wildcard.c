/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoad <jgoad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/30 16:54:22 by jgoad             #+#    #+#             */
/*   Updated: 2022/05/31 17:26:26 by jgoad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>

#include "library.h"

//#include "minishell.h"
/* Wildcard rules:


- When in between other characters, wildcard will search for variations of ONLY the character it represents
- When used by itself, wildcard will find ALL results in current directory
- When used at the start of a string, it will search for all files ending in the exact string
- When used at the end of string, it will search for all files starting with the exact string


- Wildcard can represent any amount of characters, including 0. So "testing" will show up when using test*ing

Redirection:
- Wildcard CANNOT be used in a redirect

Command names:
- Wildcard can be used as part of a command name, but will will only attempt to launch the first matching result (even if not a program)

Command arguments:
- Wildcard will be expanded into all 


To implement wildcard search:
	- Run a string compare that checks either the start of a string, the end, or both. Only the end in question need to match, 
	any number of characters can be in place of	the asterisk






Specific behaviours with functions:

- Functions that take an input file as an argument will only use the first result returned.


- If wildcard is used by itself it will attempt to run command for only the first result that is valid
	e.g. "cat *"
	- This order is based on the order in the directory
	- This only applies to certain commands. Chmod will apply to ALL files in the directory
	- ls will list all matching files



*/


// /* Check if any of the arguments of a command contain wildcards and expand */
// t_cmd * check_wildcard(t_cmd *cmd)
// {
// 	int	i;

// 	i = 0;
// 	while (cmd->args[i])
// 	{
// 		if (is_set('*', cmd->args[i]))	/* Check if a wildcard exists in string */
// 			//Expand wildcard and return array of strings.
// 			//Insert the array of strings into the original args list at current point
// 		i++;
// 	}

// }

/* Check if any of the arguments of a command contain wildcards and expand */
int	main(int argc, char **argv)
{
	char **result;
	if (is_set('*', argv[1]))
		result = expand_wildcard(argv[1]);
	
	int i = 0;
	while (result[i])
	{
		printf("%s\n", result[i]);
		i++;
	}
}

/* If a wildcard is found, expand and return an array of strings containing all results */
char **expand_wildcard(char *arg)
{
	//Start at back of string and check if there are any forward slashes
	int		i;
	char	*str;
	char	*path;
	char	**raw;
	char	**search;
	int		ends[2];

	i = ft_strlen(arg);
	while (i >= 0)
	{
		if (arg[i] == '/')
			break;
		i--;
	}
	if (i != -1)	/* If a slash found */
	{
		str = get_last_token(arg, '/');	/* Copy non-path segment */
		arg[i + 1] = '\0';
		path = ft_strdup(arg[i]);		/* Copy the path segment */
	}
	else
	{
		str = arg;
		path = NULL;
	}

	/* Check for end wildcards */
	check_ends(str, &ends);

	/* Split remaining string using asterisk */
	search = ft_split(str, '*');

	/* Open and read directory using path information, return the names of all files/folders */
	if (path)	/* If there is a relative path use for getting folder */
		raw = read_directory(path);
	else
		raw = read_directory(".");


	/* Run search on directory results, return only matching results. If null returned there are no results */
	raw = search_directory(raw, search, ends)
}

void	check_ends(char *str, int **ends)
{
	if (str && str[0] == '*')
		*ends[0] = 1;
	else
		*ends[0] = 0;
	
	if (str && str[ft_strlen(str - 1)] == '*')
		*ends[1] = 1;
	else
		*ends[1] = 0;
}

/* Search the directory array using wildcard search array */
char **search_directory(char **direct, char **search, int *ends)
{
	char	**ret;
	int	i;

	i = 0;
	while (direct[i])
	{
		if (is_wildcard_match(direct[i], search, ends);
			ret = add_str_array(ret, direct[i]);
		i++;
	}
	return (ret);
}

bool	is_wildcard_match(char *direct, char **search, int *ends)
{
	int	start;
	int end;
	int	search_tot;
	int arr_size;
	char	*ret;

	start = 0;
	end = count_array(search) - 1;
	//If ends[0] = 0 then match first token ONLY to start
	//If ends[1] = 0 then match first token ONLY to end

	/*---------------- HANDLE NO SEARCH TERMS ------------------*/
	if (!search[0])
		return (true);
	
	/*---------------- HANDLE INSUFFICIENT LENGTH --------------*/

	if (ft_strlen(direct) < get_search_tot(search))					/* If total length of string is not equal to or greater than sum of search strings return false */
		return (false);
	arr_size = count_array(search);


	/*----------------- HANDLE END MATCHES --------------------*/


	if (ends[0] == 0)												/* If no starting wildcard and no match at start return false */
	{
		if (ft_strncmp(direct, search[start], ft_strlen(search[start])))
			return (false);
		direct = direct + ft_strlen(search[start]);						/* Shorten string from front */
		start++;
	}
	if (ends[1] == 0)												/* If no ending wildcard and no match end return false */
	{
		if (ft_strncmp(direct + (ft_strlen(direct) - ft_strlen(search[end])), search[end], ft_strlen(search[end])))
			return (false);
		str[ft_strlen(direct) - ft_strlen(search[end])] = '\0';		/* Shorten string from back */
		end--;
	}

	/*----------------  HANDLE MIDDLE MATCHES ---------------------*/
	while (start <= end)
	{
		//Find a match for the token
		ret = ft_strnstr(direct, search[start], ft_strlen(direct));
		if (!ret)
			return (false);
		direct = ret;
		direct = direct + ft_strlen(search[start]);					/* Shorten string */
		start++;
	}
	return (true);
}

/* Get total string length of all search tokens */
int	get_search_tot(char **search)
{
	int	i;
	int count;

	i = 0;
	count = 0;
	while (search[i])
	{
		count += ft_strlen(search[i]);
		i++;
	}
	return (count);
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
		return (NULL);
	}
	direct = readdir(fd);
	if (!direct)
	{
		//Error messaging
		return (NULL);
	}
	while (direct)
	{
		add_array(ret, direct->d_name);
		direct = readdir(fd);
	}
	return (ret);
}

/* Add a new string to an array of strings */
char **add_str_array(char **array, char *str)
{
	char **ret;
	int	i;

	i = 0;
	ret = (char **)malloc(sizeof(char *) * (count_array(array) + 2));
	while (array[i])
	{
		ret[i] = ft_strdup(array[i]);
		i++;
	}
	ret[i] = ft_strdup(str);
	ret[i + 1] = NULL;
	free_array(array);
	return (ret);
}
