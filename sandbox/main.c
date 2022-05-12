#include "parse_test.h"

int	parse_error(char err_char)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	if (err_char == '\n')
		write(2, "newline", 7);	
	else
		write(2, &err_char, 1);	
	write(2, "\'\n", 2);
	return (err_char);
}

int	main(void)
{
	t_shell	*sh;
	int		fd;
	char	err_char;

	fd = open("input.txt", O_RDONLY);
	sh = init_shell();
 	sh->line = get_line(fd);
	err_char = 0;
	while (sh->line)
	{
		printf("#### original string: \n%s\n\n", sh->line);
		err_char = parse_test(sh, sh->line);
		if (err_char && parse_error(err_char))
			break;
		free(sh->line);
 		sh->line = get_line(fd);
	}
	print_cmds_info(sh);
	close(fd);
	cleanup_shell(sh);
	exit (err_char);
}
