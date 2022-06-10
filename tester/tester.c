#include "tester.h"

/* Run command using minishell */
void	run_line_ms(t_tester *cont, char *test_line, int fd, char **envp)
{
	dup2(fd, STDOUT_FILENO); /* Send output for this fork to log fd */
	dup2(fd, STDERR_FILENO);
	execve(MS_PATH, cont->argv_ms, envp);
}

/* Run command using bash */
void	run_line_bash(t_tester *cont, char *test_line, int fd, char **envp)
{
	dup2(fd, STDOUT_FILENO); /* Send output for this fork to log fd */
	dup2(fd, STDERR_FILENO);
	execve(BASH_PATH, cont->argv_bash, envp);

}


/* Test control function */
int	run_tests(t_tester *cont, char *test_line, char **envp)
{
	int	fd[2];
	int	ret;
	int status[2];
	char stat_str[2][1025];

	/* Open file log output files */
	fd[0] = open(MS_LOG, O_CREAT | O_RDWR, 0644);
	fd[1] = open(BASH_LOG, O_CREAT | O_RDWR, 0644);

	/* Fork processed to run command in bash and minishell */
	cont->pids[0] = fork();
	if (!cont->pids[0])
	{
		run_line_ms(cont, test_line, fd[0], envp);
		free((void **)cont->argv_ms);
		exit(0);	
	}
	cont->pids[1] = fork();
	if (!cont->pids[1])
	{
		run_line_bash(cont, test_line, fd[1], envp);
		free_array((void **)cont->argv_bash);
		exit(0);
	}
	/* Close log output files */
	close(fd[0]);
	close(fd[1]);

	/* Get return statuses and write to log value files*/
	waitpid(cont->pids[0], &status[0], 0);
	waitpid(cont->pids[1], &status[1], 0);
	fd[0] = open(MS_VAL_LOG, O_CREAT | O_RDWR, 0644);
	fd[1] = open(BASH_VAL_LOG, O_CREAT | O_RDWR, 0644);
	
	
	sprintf(stat_str[0], "%d", status[0] >> 8);
	sprintf(stat_str[1], "%d", status[1] >> 8);
	write(fd[0], stat_str[0], strlen(stat_str[0]));
	write(fd[1], stat_str[1], strlen(stat_str[1]));

	close(fd[0]);
	close(fd[1]);

	return (0);
}

/* Setup input arguments to pass to tester */
void	setup_input(t_tester *cont, int argc, char **argv, int ctrl)
{
	char **arr;

	/* argc will always be 2 */
	/* arr[0] is name, arr[1] is "-c", arr[2] is string, arr[3] is NULL */
	arr = malloc(sizeof(char *) * 4);
	if (ctrl == MINISHELL)
		arr[0] = strdup("minishell");
	else
		arr[0] = strdup("bash");
	arr[1] = strdup("-c");
	arr[2] = strdup(argv[1]);
	arr[3] = NULL;

	/* Assign result to proper array */
	if (ctrl == MINISHELL)
		cont->argv_ms = arr;
	else
		cont->argv_bash = arr;
}


int	main(int argc, char **argv, char **envp)
{	
	t_tester	cont;

	/* Check input */
	if (argc != 2)
	{
		printf("ms_tester: Invalid input\nUsage: ./ms_tester \"line_to_test\"\n");
		return (0);
	}

	mkdir("log", 0755);
	/* First arg needs to be program name, then -c, then input string */
	setup_input(&cont, argc, argv, MINISHELL);
	setup_input(&cont, argc, argv, BASH);

	/* Run test cases */
	run_tests(&cont, argv[1], envp);

	/* Kill processes, free memory, and exit */
	kill(cont.pids[0], SIGTERM);
	kill(cont.pids[1], SIGTERM);

	free_array((void **)cont.argv_ms);
	free_array((void **)cont.argv_bash);
	return (0);
}

/* Free a null terminated 2d array */
void	free_array(void **array)
{
	int	i;

	i = 0;
	if (!array)
		return ;
	while (array[i])
	{
		free(array[i]);
		array[i] = NULL;
		i++;
	}
	free(array);
	array = NULL;
}

/* 
- Test command strings should be passed with each element in a seperate arg so
	there is no need for parsing.
- Simply add -c as the first command argument and then pass the array to execve


*/
