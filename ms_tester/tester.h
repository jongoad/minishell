#ifndef TESTER_H
# define TESTER_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>

# define MS_LOG "ms.log"
# define BASH_LOG "bash.log"

typedef struct s_tester
{
	char	**argv;
	int		pids[2];
}	t_tester;

#endif