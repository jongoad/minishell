# minishell
A 42 cursus project to recreate a basic terminal shell

## About this project
The goal of minishell is to create a minimal terminal shell that functions in a similar way to bash. This was done by creating an effective and robust parsing system that is able to handle human input errors in a bash-like way, as well as to handle execution of sub-processes depending on the requirements of the parsed commands. Handling return values and errors, including system signals (SIGQUIT, SIGTERM, etc.) is imporant to the success of the project. In addition, the bonus sections required a rethinking of both the parsing and execution stages in order to handle the logic and order-of-operations.

## Team
Minishell is a team project written by @jong and @oddtiming. We separated the work into parsing and exectution, but were both heavily involved in all aspects of the code.

## Features
Our shell can handle all bash-like input excluding the use of some special characters such as ('&', ';'). 
Supported input includes: 
- pipes ('|')
- redirection ('<', '>')
- heredoc input ('<<')
- output in append mode ('>>')
- logic operators ('&&', '||') - BONUS
- parenthesis ('(', ')') - BONUS
- Wildcard operator ('*') - BONUS

We created a number of builtin functions that are simplified versions of system functions. These include:
- cd (handles relative and absolute paths)
- pwd (without and flags/options)
- echo (with or without the -n flag)
- export (without any flags/options)
- unset (without any flags/options)
- exit (will take any inputs and function as bash does)
- env (without any flags/options)


In addition, it can handle command input on shell launch:
	e.g. ./minishell "echo Hello World"

As well as single command mode (functions the same as bash -c):
	e.g. ./minishell "-c" "echo Hello World"

Return values in almost all cases are identical to bash, including when processes are interrupted with CTRL-C.
There is also a functioning readline history that will persist between sessions (it will be cleared when running "make fclean").
As the GNU readline library is included in the project dependancy files, it does not need to be installed to run this shell.

## Approach
After program launch and initialization is complete, a readline loop is entered in order to wait for user input (unless the shell was launched with a command or the -c option).
When input is received, it is passed to the parsing functions to be converted into instructions for the shell to exectute. The parsing routine uses tokenization to break the input into smaller chunks based on key characters, as well as the specific circumstances in which characters are found. During this stage, if any parse errors are found, an error message will be printed and the shell will return the use to the readline loop.

If no errors are found during parsing, the commands and all other information (redirection etc.) are converted into a struct format and sent to the execution routines.
Builtin functions are executed within the scope of the program, and will function similar to bash (e.g. export or unset in a pipeline will not affect the env of the main shell instance). All other commands are executed using execve(). Return values are monitored after execution, and will be stores in $? as with bash.

## Usage
Mandatory section of the project (no logic operators, parenthesis, or wildcards):

Build: 
	make
Run:
	./minishell

Bonus section:

Build:
	make bonus
Run:
	./minishell_bonus