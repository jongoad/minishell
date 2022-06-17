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
		// Deal with parentheses
		// For redirections after parenthesis, simply run parse() three times.
		//	Once for the content of the parenthesis
		//	Once for redirections before
		//	Once for redirections after
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

int	parse_jobs(t_shell *sh, char *line)
{
	char	*job;
	int		i;

	if (line)
		skip_whitespaces(&line);
	if (!line || !*line)
		return (EXIT_SUCCESS);
	i = 0;
	job = get_job_string(&line);
	while (job)
	{
		printf("job[%d] = %s\n", i++, job);
		free(job);
		while (*line == '&' || *line == '|')
			line++;
		job = get_job_string(&line);
	}
	printf("job[%d] = %s\n", i++, job);
	free(job);
	job = get_job_string(&line);
	printf("job[%d] = %s\n", i++, job);
	free(job);
	job = get_job_string(&line);
	printf("job[%d] = %s\n", i++, job);
	free(job);
	job = get_job_string(&line);
	printf("job[%d] = %s\n", i++, job);
	free(job);
	(void)sh;
	return (0);
}