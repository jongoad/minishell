#include "./parse_test.h"

void	print_redirs(t_cmd *cmd)
{
	int	i;

	printf(GREEN);
	if (cmd->nb_ins)
		printf("\n-------- IN FILES --------\n");
	else
		printf(RED"*** NO IN FILE ***\n"GREEN);
	i = 0;
	while (i < cmd->nb_ins)
	{
		if (cmd->ins[i]->infile)
			printf("in[%d]: \"%s\"\n", i, cmd->ins[i]->infile);
		if (cmd->ins[i]->delimiter)
			printf(RED"------> file is HEREDOC; delimiter: \"%s\"\n"GREEN, cmd->ins[i]->delimiter);
		i++;
	}
	if (cmd->nb_outs)
		printf("\n-------- OUT FILE --------\n");
	else
		printf(RED"*** NO OUT FILE ***\n"GREEN);
	i = 0;
	while (i < cmd->nb_outs)
	{
		printf("in[%d]: \"%s\"\n", i, cmd->outs[i]->outfile);
		if (cmd->outs[i]->append_mode)
			printf(RED"------> file is O_APPEND\n"GREEN);
		i++;
	}
	printf(RESET_COL);
}

void	print_cmd_args(t_cmd *cmd)
{
	int	i;

	printf(MAGENTA);
	if (!cmd->nb_args)
	{
		printf(RED"*** NO CMD_ARGS *** \n"RESET_COL);
		return ;
	}
	printf("-------- CMD_ARGS --------\n");
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
		printf(CYAN);
		if (sh->cmds[i]->exe)
			printf("\n\tcmd #%i: \"%s\"\n", i + 1, sh->cmds[i]->exe);
		else
			printf("*** NO CMD_FILEPATH*** \n");
		printf(RESET_COL);
		print_cmd_args(sh->cmds[i]);
		print_redirs(sh->cmds[i]);
		i++;
	}
	return ;
}
