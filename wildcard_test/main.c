#include "./includes/library.h"

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
	free(arg);
	free_array((void **)result);
}
