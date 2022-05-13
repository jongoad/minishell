# include <dirent.h>
# include <curses.h>
# include <term.h>
# include <stdlib.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/types.h>
# include <stdio.h>
# include <sys/stat.h>
# include <sys/ioctl.h>
# include <termios.h>
# include <signal.h>
# include <fcntl.h>
# include <string.h>
# include <sys/wait.h>
# include <sys/time.h>
# include <sys/resource.h>
# include <stdbool.h>
# include <errno.h>


/* Read a string, and return a pointer to it.  Returns NULL on EOF. */
char *get_line(char *line)
{
	if (line)
	{
    	free(line);
    	line = (char *)NULL;
  	}
	line = readline ("");		/* Get a line from the user. */
	if (line && *line)		/* If the line has any text in it, save it on the history. */
		add_history (line);
  return (line);
}

int	main(void)
{
	char *line = (char *)NULL;

	while (1)
	{
		line = get_line(line);
		printf("Line: %s\n", line);
	}

}


void	rl_clear_history(void);

int	rl_on_new_line(void);
//Tell the update routines that we have moved onto a new (empty) line, usually after ouputting a newline.
void rl_redisplay(void);
//Change what's displayed on the screen to reflect the current contents of rl_line_buffer.
void	rl_replace_line(const char *text, int clear_undo);
//Replace the contents of rl_line_buffer with text. The point and mark are preserved, if possible. If clear_undo is non-zero, the undo list associated with the current line is cleared.
