#include "minishell.h"

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

/* Check if there are wildcard characters at either the start or end of the string */
void	check_ends(char *str, int *ends)
{
	if (str && str[0] == WILDCARD)
		ends[0] = 1;
	else
		ends[0] = 0;
	
	if (str && str[ft_strlen(str) - 1] == WILDCARD)
		ends[1] = 1;
	else
		ends[1] = 0;
}

/* Free a string and return bool */
bool free_return_bool(char *str, bool ret)
{
	free(str);
	return (ret);
}

/* Join two strings and free the second string */
char	*ft_strjoin_free_rev(char *s1, char *s2)
{
	size_t	tot_len;
	char	*s_joined;
	int		i;

	tot_len = ft_strlen(s1) + ft_strlen(s2) + 1;
	s_joined = malloc(tot_len);
	if (!s_joined)
		return (NULL);
	i = 0;
	if (s1)
	{
		while (s1[i])
			*s_joined++ = s1[i++];
	}
	i = 0;
	if (s2)
		while (s2[i])
			*s_joined++ = s2[i++];
	*s_joined++ = 0;
	free(s2);
	return (s_joined - tot_len);
}
