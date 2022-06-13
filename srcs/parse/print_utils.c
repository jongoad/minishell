#include "minishell.h"

void	print_ins(t_cmd *cmd)
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
		if (cmd->ins[i]->delim)
			printf(RED"------> in[%d] is HEREDOC; delim: \"%s\"\n"GREEN,
				i, cmd->ins[i]->delim);
		i++;
	}
}

void	print_outs(t_cmd *cmd)
{
	int	i;

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
		print_ins(sh->cmds[i]);
		print_outs(sh->cmds[i]);
		i++;
	}
	return ;
}

void	print_char_arr(char **arr)
{
	int	i;

	i = 0;
	if (!arr)
	{
		printf("arr is (null)\n");
		return ;
	}
	if (!*arr)
	{
		printf("arr[0] is (null)\n");
		return ;
	}
	while (arr[i])
	{
		printf("arr[%d] = %s\n", i, arr[i]);
		i++;
	}
}
