#include "parse_test.h"

int	main(void)
{
	t_shell	*sh;
	int		fd;

	fd = open("input.txt", O_RDONLY);
	sh = init_shell();
 	sh->line = get_line(fd);
	while (sh->line)
	{
		parse_test(sh);
		free(sh->line);
 		sh->line = get_line(fd);
	}
	close(fd);
	cleanup(sh);
	return (0);
}
