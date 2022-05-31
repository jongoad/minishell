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
t_shell *init_shell(t_shell *sh, int argc, char **argv, char **envp)
{
	sh = get_data();
	init_shell_prompt(sh, argv[0]);			/* Set shell name and prompt */
	init_env_vars(sh, envp);				/* Set environment vars array and paths array */
	init_builtins(sh);						/* Initialize builtin function array and function pointers */
	init_history(sh);
	sh->ret_val = 0;
	sh->line = (char *)NULL;
	sh->cmd_line = false;
	
	/* Ismael: Commented out temporarily for debugging */
	// if (argc > 1)							/* If a command is passed with shell, need to send it to be parsed as first command string */
	// 	return (NULL);
	
	(void)argc;
	return (sh);
}
//FIX error handling


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

/* Update shell level for current shell */
char	*increment_shlvl(char *str)
{
	char	*tmp;
	char	*num;

	while (*str && *str != '=')
		str++;
	str++;

	num = ft_itoa(ft_atoi(str) + 1);
	tmp = ft_strjoin("SHLVL=", num);
	free(num);
	return (tmp);
	
}

/* Init builtin commands struct */
void	init_builtins(t_shell *sh)
{
	/* Init alias strings */
	sh->builtins.alias[0] = "echo";
	sh->builtins.alias[1] = "cd";
	sh->builtins.alias[2] = "pwd";
	sh->builtins.alias[3] = "export";
	sh->builtins.alias[4] = "unset";
	sh->builtins.alias[5] = "env";
	sh->builtins.alias[6] = "exit";

	/* Init fucntion pointer array */
	sh->builtins.f[0] = &builtin_echo;
	sh->builtins.f[1] = &builtin_cd;
	sh->builtins.f[2] = &builtin_pwd;
	sh->builtins.f[3] = &builtin_export;
	sh->builtins.f[4] = &builtin_unset;
	sh->builtins.f[5] = &builtin_env;
	sh->builtins.f[6] = &builtin_exit;
}
