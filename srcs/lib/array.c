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