#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

int main(void)
{
	char *buf;
	const char prompt[12] = "Minishell->";

	buf = readline(prompt);

	printf("You entered: %s", buf);
}

// MUST COMPILEWITH -lreadline