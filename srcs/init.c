#include "minishell.h"

/* Singleton to get shell struct */
t_shell	*get_data(void)
{
	static t_shell	*sh;

	if (!sh)
		sh = ft_xalloc(sizeof(t_shell));
	return (sh);
}

/* Initialize shell variables and do preliminary setup */
t_shell *init_shell(t_shell *sh, char **argv, char **envp)
{
	sh = get_data();
	init_shell_prompt(sh, argv[0]);			/* Set shell name and prompt */
	init_env_vars(sh, envp);				/* Set environment vars array and paths array */
	init_builtins(sh);						/* Initialize builtin function array and function pointers */
	init_history(sh);
	init_pwd(sh);
	sh->ret_val = 0;
	sh->line = (char *)NULL;
	sh->cmd_line = false;
	return (sh);
}

/* Initialize prompt and shell name */
void	init_shell_prompt(t_shell *sh, char *name)
{
	sh->sh_name = get_last_token(name, '/');
	sh->prompt = ft_strjoin(sh->sh_name, "-> ");
}

/* Initialize environment variables */
void	init_env_vars(t_shell *sh, char **envp)
{
	int i;

	i = 0;
	sh->env.envp = ft_xalloc(sizeof(char *) * (count_array((void **)envp) + 1));
	sh->env.envp[count_array((void **)envp)] = NULL;
	while (*envp)
	{
		sh->env.envp[i] = ft_strdup(*envp);
		if (!ft_strncmp(*envp, "PATH=", 5))
		{
			sh->env.path = ft_split(*envp + 5, ':');
		}
		else if (!ft_strncmp(*envp, "SHLVL=", 6))
		{
			free(sh->env.envp[i]);
			sh->env.envp[i] = increment_shlvl(*envp);
		}
		envp++;
		i++;
	}
}

/* Reinit path every time a function is called */
void	init_path(t_shell *sh)
{
	int	i;

	i = 0;
	if (sh->env.path)
	{
		free(sh->env.path);
		sh->env.path = NULL;
	}
	while (sh->env.envp[i])
	{
		if (!ft_strncmp(sh->env.envp[i], "PATH=", 5))
		{
			sh->env.path = ft_split(sh->env.envp[i] + 5, ':');
		}
		i++;
	}
}
