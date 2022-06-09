#ifndef TESTER_H
# define TESTER_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <string.h>
# include <stdlib.h>
# include <signal.h>

# define MS_LOG "./log/ms.log"
# define BASH_LOG "./log/bash.log"
# define MS_VAL_LOG "./log/ms_val.log"
# define BASH_VAL_LOG "./log/bash_val.log"

# define MS_PATH "../minishell"
# define MS_PATH_BONUS "../minishell_bonus"
# define BASH_PATH "/bin/bash"


enum control
{
	MINISHELL = 0,
	BASH = 1,
};

typedef struct s_tester
{
	char	**argv_ms;
	char	**argv_bash;
	int		pids[2];	
}	t_tester;

/* Tester functions */
void	run_line_ms(t_tester *cont, char *test_line, int fd, char **envp);
void	run_line_bash(t_tester *cont, char *test_line, int fd, char **envp);
int		run_tests(t_tester *cont, char *test_line, char **envp);
void	setup_input(t_tester *cont, int argc, char **argv, int ctrl);


/* Utilities */
void	free_array(void **array);
#endif