#include "./parse_test.c"

void	free_split(void **split)
{
	int	i;

	if (!split)
		return ;
	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free (split);
	return ;
}
