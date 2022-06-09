#include "../../includes/minishell.h"

/* Count the number of elements in a 2d array that is null terminated */
int	count_array(void **array)
{
	int count;

	count = 0;
	while (array && array[count])
		count++;
	return (count);
}

/* Free a null terminated 2d array */
void	free_array(void **array)
{
	int	i;

	i = 0;
	if (array)
	{
		while(array[i])
		{
			free(array[i]);
			array[i] = NULL;
			i++;
		}
		free(array);
		array = NULL;
	}
}

/* Add a new string to an array of strings */
char **add_str_array(char **array, char *str)
{
	char **ret;
	int	i;

	i = 0;
	ret = ft_xalloc(sizeof(char *) * (count_array((void **)array) + 2));
	while (array && array[i])
	{
		ret[i] = ft_strdup(array[i]);
		i++;
	}
	ret[i] = ft_strdup(str);
	free_array((void **)array);
	array = NULL;
	return (ret);
}

/* Join two string arrays together */
char	**join_array_array(char **arr1, char **arr2)
{
	char	**result;
	int		count;
	int		i;
	int		j;

	result = NULL;
	count = count_array(((void **)arr1)) + count_array((void**)arr2) + 1;
	result = ft_xalloc(sizeof(char *) * count);
	i = 0;
	while (arr1 && arr1[i])
	{
		result[i] = ft_strdup(arr1[i]);
		i++;
	}
	j = 0;
	while (arr2 && arr2[j])
	{
		result[i] = ft_strdup(arr2[j]);
		i++;
		j++;
	}
	free_array((void **)arr1);
	free_array((void **)arr2);
	return (result);
}