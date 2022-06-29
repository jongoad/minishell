#include "minishell_bonus.h"

int	get_job_len(char *str)
{
	int	job_len;
	int	parenthesis_len;

	job_len = 0;
	while (*str)
	{
		job_len += skip_quotes(&str);
		if (*str == '(')
		{
			parenthesis_len = get_parenthesis_len(str);
			str += parenthesis_len;
			job_len += parenthesis_len;
		}
		if (*str == '&' || (*str == '|' && *(str + 1) == '|'))
			break ;
		str++;
		job_len++;
	}
	return (job_len);
}

char	*get_job_string(char **line)
{
	char	*job;
	int		job_len;

	if (!line || !*line || !**line)
		return (NULL);
	job_len = get_job_len(*line);
	job = ft_xalloc(job_len + 1);
	// printf("job_len = %d\n", job_len);
	job = ft_strncpy(job, *line, job_len);
	*line += job_len;
	// *line = ptr;
	// job[job_len] = '\0';
	// while (job_len)
	// 	job[--job_len] = *(--ptr);
	return (job);
}

void	steal_job(t_shell *sh, char *line)
{
	t_ms_job	*new;

	new = ms_jobnew(sh->cmds, sh->nb_cmds, *line);
	ms_jobadd(&sh->jobs, new);
	sh->cmds = NULL;
	sh->nb_cmds = 0;
}

int	parse_jobs(t_shell *sh, char *line)
{
	char	*job_string;
	char	*line_ptr;
	// int		i;

	if (!line || !*line)
		return (EXIT_SUCCESS);
	line_ptr = line;
	if (line_ptr)
		skip_whitespaces(&line_ptr);
	// i = 0;
	job_string = get_job_string(&line_ptr);
	while (job_string)
	{
		// printf("job[%d] = %s\n", i++, job_string);
		parse(sh, job_string);
		cmds_lst_to_str(sh);
		// print_cmds_info(sh);
		steal_job(sh, line_ptr);
		/* This is the risky part, literally just taking cmds and storing it in jobs
			The idea is to simply replace shell's cmds pointer by current job's */
		free(job_string);
		while (*line_ptr == '&' || *line_ptr == '|')
			line_ptr++;
		job_string = get_job_string(&line_ptr);
	}
	return (0);
}
