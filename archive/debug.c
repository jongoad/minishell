#include "../includes/minishell.h"



/* Testing data for debug_execute */
void	setup_debug_execute(t_shell *sh)
{
	/* Setup commands */
	char **cmds;
	cmds = ft_split("cat", ' ');
	
	/* Setup args */
	int nb_args = 1;
	char ***args = malloc(sizeof(char **) * (nb_args));
	args[0] = ft_split("cat", ' ');
	//args[1] = ft_split("wc -l", ' ');
	//args[2] = ft_split("wc", ' ');

	/* Setup infiles */
	char ***in = malloc(sizeof(char **) * 1);
	in[0] = ft_split("tests.txt", ' ');
	// in[1] = ft_split("test1 test2", ' ');
	// in[2] = malloc(sizeof(char *));
	// in[2] = NULL;
	// in[1] = malloc(sizeof(char *));
	// in[1] = NULL;
	// in[0] = malloc(sizeof(char *));
	// in[0] = NULL;
	// in[2] = malloc(sizeof(char *));
	// in[2] = NULL;
	/* Setup outfiles */
	char ***out = malloc(sizeof(char **) * 1);
	out[0] = ft_split("out.txt", ' ');
	// out[1] = ft_split("test1 test2", ' ');
	// out[0] = malloc(sizeof(char *));
	// out[0] = NULL;
	// out[1] = malloc(sizeof(char *));
	// out[1] = NULL;
	// out[2] = malloc(sizeof(char *));
	// out[2] = NULL;
	// out[2] = malloc(sizeof(char *));
	// out[2] = NULL;

	debug_execute(sh, cmds, args, in, out);
}

/* Set up test inputs for execution */
void	debug_execute(t_shell *sh, char **cmds, char ***args, char ***in, char ***out)
{
	sh->nb_cmds = count_array((void**)cmds);
	sh->cmds = malloc(sizeof(t_cmd) * sh->nb_cmds + 1);
	sh->cmds[sh->nb_cmds] = NULL;
	int i = 0;
	while (cmds[i])
	{
		sh->cmds[i] = create_command(sh, cmds[i], args[i], in[i], out[i]);
		i++;
	}
	sh->pids = malloc(sizeof(pid_t) * sh->nb_cmds);
}

/* Create a command for debuf purposes using inputs */
t_cmd *create_command(t_shell *sh, char *cmd, char **args, char **in, char **out)
{
	t_cmd *res;

	res = malloc(sizeof(t_cmd));		/* Allocate a new command */
	res->exe = cmd;				/* Set cmd name/path */
	res->envp = sh->env.envp;			/* Initialize envp */
	res->errnum = 0;					/* Initialize errnum to 0 */
	res->args = args;					/* Set args array to input */
	res->nb_args = count_array((void **)args);	/* Set nb_args */

	/* Setup redirection */
	res->ins = malloc(sizeof(t_infile) * count_array((void **)in));
	res->outs = malloc(sizeof(t_outfile) * count_array((void **)out));

	int i = 0; 
	int j = 0;
	/* Setup infiles */
	while (in && *in)
	{
		res->ins[i].infile = *in;
		res->ins[i].fd = -1;
		i++;
		in++;
	}
	res->ins[i].fd = -1;
	res->nb_ins = i;
	/* Setup outfiles */
	while (in && *out)
	{
		res->outs[j].append_mode = 0;
		res->outs[j].outfile = *out;
		res->outs[j].fd = -1;
		j++;
		out++;
	}
	res->outs[j].fd = -1;
	res->nb_outs = j;

	return (res);
}
// Dont worry about here_doc or multiple redirects for now
// make sure all input arrays are null terminated


void	print_debug_data(t_shell *sh)
{
	int j = 0;
	int k;
	printf("Number of commands: %i\n", sh->nb_cmds);

	while (j < sh->nb_cmds)
	{
		printf("Command #: %i\n", j + 1);
		printf("Command name/path: %s\n", sh->cmds[j]->exe);

		printf("Command arguments:\n");
		k = 0;
		while (k < sh->cmds[j]->nb_args)
		{
			printf("Arg %i: %s\n", k, sh->cmds[j]->args[k]);
			k++;
		}
		//printf("\n\n");
		printf("Infiles:\n");
		k = 0;
		while (k < sh->cmds[j]->nb_ins)
		{
			printf("%s\n", sh->cmds[j]->ins[k].infile);
			k++;
		}
		//printf("\n\n");
		printf("Outfiles:\n");
		k = 0;
		while (k < sh->cmds[j]->nb_outs)
		{
			printf("%s\n", sh->cmds[j]->outs[k].outfile);
			k++;
		}
		//printf("\n\n");
		j++;
	}

}