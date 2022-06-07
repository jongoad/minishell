/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoad <jgoad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/30 16:54:22 by jgoad             #+#    #+#             */
/*   Updated: 2022/06/02 16:09:21 by jgoad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "library.h"


//#include "minishell.h"

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
	char **result = NULL;
	char *arg = ft_strdup(argv[1]);
	
	if (is_set('*', arg))
		result = expand_wildcard(arg);
	
	if (result)
	{
		int i = 0;
		while (result && result[i])
		{
			printf("%s\n", result[i]);
			i++;
		}
	}
	free_array((void **)result);
}

bool free_return_bool(char *str, bool ret)
{
	free(str);
	return (ret);
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
	int		*ends;

	
	ends = malloc(sizeof(int) * 2);

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
		path = ft_strdup(arg);		/* Copy the path segment */
	}
	else
	{
		str = arg;
		path = NULL;
	}
	//printf("%s\n", str);
	/* Check for end wildcards */
	check_ends(str, ends);
	/* Split remaining string using asterisk */
	search = ft_split(str, '*');
	/* Open and read directory using path information, return the names of all files/folders */
	if (path)	/* If there is a relative path use for getting folder */
		raw = read_directory(path);
	else
		raw = read_directory(".");
	
	printf("ends[0]: %i ends[1]: %i\n", ends[0], ends[1]);

	/* Run search on directory results, return only matching results. If null returned there are no results */
	raw = search_directory(raw, search, ends);

	/* Free memory before exit */
	if (path)
		free(path);
	if (str)
		free(str);
	free(ends);
	free_array((void **)search);
	return (raw);
}

void	check_ends(char *str, int *ends)
{
	if (str && str[0] == '*')
		ends[0] = 1;
	else
		ends[0] = 0;
	
	if (str && str[ft_strlen(str) - 1] == '*')
		ends[1] = 1;
	else
		ends[1] = 0;
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


//Free will not work correctly? We have incremented the pointer
bool	is_wildcard_match(char *direct, char **search, int *ends)
{
	int	start;
	int end;
	int	search_tot;
	int arr_size;
	char	*ret;
	char	*tmp;
	char	*p_tmp;


	start = 0;
	end = count_array((void **)search) - 1;
	tmp = ft_strdup(direct);
	p_tmp = tmp;
	//If ends[0] = 0 then match first token ONLY to start
	//If ends[1] = 0 then match first token ONLY to end
	/*---------------- HANDLE NO SEARCH TERMS ------------------*/
	if (!search[0])
	{
		free(p_tmp);
		return (true);
	}
	/*---------------- HANDLE INSUFFICIENT LENGTH --------------*/
	
	if (ft_strlen(tmp) < get_search_tot(search))					/* If total length of string is not equal to or greater than sum of search strings return false */
	{
		free(p_tmp);
		return (false);
	}
	arr_size = count_array((void **)search);


	/*----------------- HANDLE END MATCHES --------------------*/


	if (ends[0] == 0)												/* If no starting wildcard and no match at start return false */
	{
		if (ft_strncmp(tmp, search[start], ft_strlen(search[start])))
		{
			free(p_tmp);
			return (false);
		}
		tmp = tmp + ft_strlen(search[start]);						/* Shorten string from front */
		start++;
	}
	if (ends[1] == 0)												/* If no ending wildcard and no match end return false */
	{
		if (ft_strncmp(tmp + (ft_strlen(tmp) - ft_strlen(search[end])), search[end], ft_strlen(search[end])))
		{
			// tmp = p_tmp;
			free(p_tmp);
			return (false);
		}
		tmp[ft_strlen(tmp) - ft_strlen(search[end])] = '\0';		/* Shorten string from back */
		end--;
	}

	/*----------------  HANDLE MIDDLE MATCHES ---------------------*/
	while (start <= end)
	{
		//Find a match for the token
		ret = ft_strnstr(tmp, search[start], ft_strlen(tmp));
		if (!ret)
		{
			// tmp = p_tmp;
			free(p_tmp);
			return (false);
		}
		if (p_tmp)
			free(p_tmp);
		tmp = ret;
		tmp = tmp + ft_strlen(search[start]);					/* Shorten string */
		start++;
	}
	// tmp = p_tmp;
	free (p_tmp);
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


/* Add a new string to an array of strings */
char **add_str_array(char **array, char *str)
{
	char **ret;
	int	i;

	i = 0;
	ret = (char **)malloc(sizeof(char *) * (count_array((void **)array) + 2));
	while (array && array[i])
	{
		ret[i] = ft_strdup(array[i]);
		i++;
	}
	ret[i] = ft_strdup(str);
	ret[i + 1] = NULL;
	free_array((void **)array);
	array = NULL;
	return (ret);
}
