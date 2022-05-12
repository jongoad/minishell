#include "parse_test.h"

void	add_infile(t_cmd *cmd, t_infile *new_in)
{
	t_infile	**new_arr;
	int			i;

	cmd->nb_ins++;
	new_arr = ft_xalloc((cmd->nb_ins + 1) * sizeof(t_infile *));
	i = 0;
	while (i < cmd->nb_ins - 1)
	{
		new_arr[i] = cmd->ins[i];
		i++;
	}
	new_arr[i++] = new_in;
	new_arr[i] = NULL;
	free(cmd->ins);
	cmd->ins = new_arr;
}

void	add_outfile(t_cmd *cmd, t_outfile *new_out)
{
	t_outfile	**new_arr;
	int			i;

	cmd->nb_outs++;
	new_arr = ft_xalloc((cmd->nb_outs + 1) * sizeof(t_outfile *));
	i = 0;
	while (i < cmd->nb_outs - 1)
	{
		new_arr[i] = cmd->outs[i];
		i++;
	}
	new_arr[i++] = new_out;
	new_arr[i] = NULL;
	free(cmd->outs);
	cmd->outs = new_arr;
}

void	parse_in(t_cmd *cmd, char *cl_tok, bool is_double)
{
	t_infile	*new_in;

	new_in = ft_xalloc(sizeof(t_infile));
	new_in->fd = -1;
	if (is_double)
		new_in->delimiter = cl_tok;
	else
		new_in->infile = cl_tok;
	add_infile(cmd, new_in);
	// if (is_double)
	// 	new_in->fd = read_heredoc();
}

void	parse_out(t_cmd *cmd, char *cl_tok, bool is_double)
{
	t_outfile	*new_out;

	new_out = ft_xalloc(sizeof(t_outfile));
	new_out->fd = -1;
	new_out->outfile = cl_tok;		
	new_out->append_mode = is_double;
	add_outfile(cmd, new_out);
}

int	parse_redir(t_cmd *cmd, char **line)
{
	char	*ptr;
	char	*cl_tok;
	bool	is_double;

	printf(YELLOW);
	printf("parse_redir()\n");

	is_double = false;
	ptr = *line + 1;
	if (*ptr == *(ptr - 1))		// Increment for `>>' or `<<'
	{
		is_double = true;
		ptr++;
	}
	printf(RESET_COL"ptr: %s\n"YELLOW, ptr);
	skip_whitespaces(&ptr);
	if (!*ptr || is_set(*ptr, "<>|"))
	{
		printf("\n\n\tfuck\n\n");
		if (is_set(*ptr, "<>|"))
			return (*ptr);			// Returns the problem char
		else if (!*ptr)
			return ('\n');			// To handle `newline' output in error msg
	}
	printf("ready for cl_tok\n");
	cl_tok = get_cl_tok(&ptr);
	printf("cl_tok = %s\n", cl_tok);
	if (**line == '<')
		parse_in(cmd, cl_tok, is_double);
	else if (**line == '>')
		parse_out(cmd, cl_tok, is_double);
	print_redirs(cmd);
	*line = ptr;

	printf(RESET_COL);

	return (0);
}
