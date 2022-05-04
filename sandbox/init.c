#include "./parse_test.h"

t_shell	*init_shell(void)
{
	t_shell	*shell;

	shell = ft_xalloc(sizeof(t_shell));
	return (shell);
}
