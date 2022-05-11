#include "./parse_test.h"

void	print_redirs(t_cmd *cmd)
{
	int	i;

	printf(CYAN);
	if (cmd->nb_ins)
		printf("\n--------> IN FILES\n");
	else
		printf("*** NO IN FILE ***\n");
	i = 0;
	// while (i < cmd->nb_ins)
	while (cmd->ins + i)
	{
		printf("in[%d]: \"%s\"\n", i, cmd->ins[i].infile);
		if (cmd->ins[i].delimiter)
			printf("\tAssociated delimiter: \"%s\"\n", cmd->ins[i].delimiter);
		i++;
	}
	printf(YELLOW);
	if (cmd->nb_outs)
		printf("\n--------> OUT FILES\n");
	else
		printf("*** NO OUT FILE ***\n");
	i = 0;
	// while (i < cmd->nb_outs)
	while (cmd->outs + i)
	{
		printf("in[%d]: \"%s\"\n", i, cmd->outs[i].outfile);
		if (cmd->outs[i].append_mode)
			printf("\tfile is O_APPEND\n");
		i++;
	}
	printf(RESET_COL);
}

void	print_cmd_args(t_cmd *cmd)
{
	int	i;

	printf(YELLOW);
	if (cmd->filepath)
		printf("\ncmd_filepath: \"%s\"\n", cmd->filepath);
	else
		printf("*** NO CMD_FILEPATH*** \n");
	printf(MAGENTA);
	if (cmd->nb_args)
		printf("\n-------- CMD_ARGS --------\n");
	else
		printf("*** NO CMD_ARGS *** \n");
	i = 0;
	// while (i < cmd->nb_args)
	while (cmd->args && cmd->args[i])
	{
		printf("arg #%d: \"%s\"\n", i, cmd->args[i]);
		i++;
	}
	printf(RESET_COL);
}

void	print_cmds_info(t_shell *sh)
{
	int	i;

	i = 0;
	while (i < sh->nb_cmds)
	{
		print_cmd_args(sh->cmds[i]);
		print_redirs(sh->cmds[i]);
		i++;
	}
	return ;
}
