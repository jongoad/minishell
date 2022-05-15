#include "minishell.h"

/* Initialize shell variables and do preliminary setup */
t_shell *init_shell(t_shell *sh, int argc, char **argv, char **envp)
{
	sh = malloc(sizeof(t_shell));
	init_shell_prompt(sh, argv[0]);			/* Set shell name and prompt */
	init_env_vars(sh, envp);				/* Set environment vars array and paths array */
	init_builtins(sh);						/* Initialize builtin function array and function pointers */
	init_history(sh);
	sh->ret_val = 0;
	sh->line = (char *)NULL;
	
	/* Ismael: Commented out temporatily */
	// if (argc > 1)							/* If a command is passed with shell, need to send it to be parsed as first command string */
	// 	return (NULL);
	
	(void)argc;
	return (sh);
}
//FIX error handling

/* Reset shell */
void reset_shell(t_shell *sh)
{
	free(sh->pids);
	free(sh->pipes);
	sh->nb_cmds = 0;
	sh->cmd_iter = 0;
	sh->nb_pipes = 0;
}
/* Initialize prompt and shell name */
void	init_shell_prompt(t_shell *sh, char *name)
{
	// int len;
	// int	i;

	// i = 0;
	// len = ft_strlen(name);
	// sh->prompt = malloc(len + 3);
	// // while (name[i] == '.' || name[i] == '/')
	// // 	i++;


	sh->sh_name = get_last_token(name, '/');
	sh->prompt = ft_strjoin(sh->sh_name, "-> ");

	// while (name[i])
	// {
	// 	sh->prompt[i] = name[i];
	// 	i++;
	// }
	// sh->prompt[i] = '-';
	// sh->prompt[i + 1] = '>';
	// sh->prompt[i + 2] = '\0';
	// sh->sh_name = ft_strdup(name);
}

/* Initialize environment variables */
void	init_env_vars(t_shell *sh, char **envp)
{
	int i;

	i = 0;
	sh->env.envp = (char **)malloc(sizeof(char *) * (count_array((void **)envp) + 1));
	while (*envp)
	{
		sh->env.envp[i] = ft_strdup(*envp);
		if (!ft_strncmp(*envp, "PATH=", 5))
		{
			sh->env.path = ft_split(*envp + 5, ':');
		}
		else if (!ft_strncmp(*envp, "SHLVL=", 6))
			sh->env.envp[6] += 1;
		envp++;
		i++;
	}
	sh->env.envp[i] = NULL;
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
