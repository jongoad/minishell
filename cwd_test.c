#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>


int main(void)
{
	char path[] = "./thingstuff";
	// char *buf;
	// buf = malloc(500);

	// buf = getcwd(buf, 500);
	// printf("%s\n", buf);

	if (chdir(path))
		perror(path);

	// buf = getcwd(buf, 500);
	// printf("%s\n", buf);

}
