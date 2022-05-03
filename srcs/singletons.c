#include "../includes/minishell.h"

t_shell *get_data(void)
{
	static t_shell *sh;

	if (!sh)
		sh = malloc(sizeof(t_shell));
	return (sh);
}

t_builtin *get_builtin(void)
{
	static t_builtin *bltin;

	if (!bltin)
		bltin = malloc(sizeof(t_builtin));
	return (bltin);
}