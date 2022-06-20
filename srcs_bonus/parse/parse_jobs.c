#include "minishell_bonus.h"

char	*get_job_string(char **line)
{
	char	*ptr;
	char	*job;
	int		job_len;

	if (!line || !*line || !**line)
		return (NULL);
	ptr = *line;
	while (*ptr && *ptr != '&' && !(*ptr == '|' && *(ptr + 1) == '|'))
	{
		printf("ptr = %c\n", *ptr);
		ptr++;
	}
	job_len = ptr - *line;
	job = ft_xalloc(job_len + 1);
	printf("job_len = %d\n", job_len);
	*line = ptr;
	job[job_len] = '\0';
	while (job_len)
		job[--job_len] = *(--ptr);
	return (job);
}

void	steal_job(sh, line)
{

}

int	parse_jobs(t_shell *sh, char *line)
{
	char	*job_string;
	char	*job_ptr;
	char	*line_ptr;
	int		i;

	if (!line || !*line)
		return (EXIT_SUCCESS);
	line_ptr = line;
	if (line_ptr)
		skip_whitespaces(&line_ptr);
	i = 0;
	while (job_string)
	{
		job_string = get_job_string(&line_ptr);
		parse(sh, job_string);
		printf("job[%d] = %s\n", i++, job_string);

		/* This is the risky part, literally just taking cmds and storing it in jobs
			The idea is to simply replace cmds->pointer by jobs */
		ms_jobadd(&sh->jobs, ms_jobnew(sh->cmds, *line_ptr));
		sh->cmds = NULL;
		free(job_string);
		while (*line_ptr == '&' || *line_ptr == '|')
			line_ptr++;
		job = get_job_string(&line_ptr);
	}
	// printf("job[%d] = %s\n", i++, job);
	// free(job);
	// job = get_job_string(&line);
	// printf("job[%d] = %s\n", i++, job);
	// free(job);
	// job = get_job_string(&line);
	// printf("job[%d] = %s\n", i++, job);
	// free(job);
	// job = get_job_string(&line);
	// printf("job[%d] = %s\n", i++, job);
	// free(job);
	// (void)sh;
	return (0);
}