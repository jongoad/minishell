#include "tester.sh"

int	run_line(t_tester *cont, char *test_line, int fd)
{
	dup2(fd, STDOUT_FILENO);

	//Need to add the line 
}

int	run_tests(t_tester *cont, char *test_line)
{
	int	fd;
	int	ret;

	cont->pids[0] = fork();
	if (!cont->pids[0])
	{
		fd = open(MS_LOG, O_CREAT | O_WRONLY);
		run_line(cont, test_line, fd);
		free(tester);
		exit(0);	
	}
	cont->pids[1] = fork();
	if (!cont->pids[1])
	{
		fd = open(BASH_LOG, O_CREAT | O_WRONLY);
		run_line(cont, test_line, fd);
		free(tester);
		exit(0);	
	}
	waitpid(cont->pids[0], NULL, WNOHANG);
	waitpid(cont->pids[1], NULL, WNOHANG);
	return (0)
}

int	main(int argc, char *argv[])
{	
	t_tester	*cont;

	if (argc < 2)
		return (0);
	tester = malloc(sizeof(t_tester));
	if (!tester)
		return (0);

	run_tests(test, argv[1]);
	kill(cont->pids[0], SIGTERM);
	free(tester);
	return (0);
}
