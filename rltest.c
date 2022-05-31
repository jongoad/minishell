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


// struct dirent {
//                ino_t          d_ino;       /* Inode number */
//                off_t          d_off;       /* Not an offset; see below */
//                unsigned short d_reclen;    /* Length of this record */
//                unsigned char  d_type;      /* Type of file; not supported
//                                               by all filesystem types */
//                char           d_name[256]; /* Null-terminated filename */
//            };



int	main(void)
{
	DIR *directory;
	struct dirent *direct;

	directory = opendir(".");

	//Check if opened
	if (!directory)
	{
		printf("Unable to open directory\n");
		exit (1);
	}
	direct = readdir(directory);
	if (!direct)
	{
		printf("Error or empty directory\n");
		exit(1);
	}
	while (direct)
	{
		printf("Inode #: %llu\n", direct->d_ino);
		printf("Size of record: %hu\n", direct->d_reclen);
		printf("Type of file: %u\n", direct->d_type);
		printf("Filename: %s\n", direct->d_name);
		printf("-----------------------------\n\n");
		direct = readdir(directory); 
	}
	closedir(directory);
}


//Type = 4 is a directory
//Type = 8 is a file