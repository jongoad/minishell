#include "minishell.h"



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

/* Free memory and return NULL on error */
char	**wildcard_error_return(t_wildcard *wc)
{
	if (wc->path)
		free(wc->path);
	if (wc->str)
		free(wc->str); 
	free(wc->ends);
	free_array((void **)wc->search);
	return (NULL);
}

/* Add wildcard character back to string */
char	*replace_wildcard(char *str)
{
	int	i;

	i = 0;
	while (str && str[i])
	{
		if (str[i] == WILDCARD)
			str[i] = '*';
		i++;
	}
	return (str);
}