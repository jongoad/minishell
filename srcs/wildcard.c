/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoad <jgoad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/30 16:54:22 by jgoad             #+#    #+#             */
/*   Updated: 2022/05/31 13:41:18 by jgoad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Wildcard rules:


- When in between other characters, wildcard will search for variations of ONLY the character it represents
- When used by itself, wildcard will find ALL results in current directory
- When used at the start of a string, it will search for all files ending in the exact string
- When used at the end of string, it will search for all files starting with the exact string


- Wildcard can represent any amount of characters, including 0. So "testing" will show up when using test*ing

Redirection:
- Wildcard CANNOT be used in a redirect

Command names:
- Wildcard can be used as part of a command name, but will will only attempt to launch the first matching result (even if not a program)

Command arguments:
- Wildcard will be expanded into all 


To implement wildcard search:
	- Run a string compare that checks either the start of a string, the end, or both. Only the end in question need to match, 
	any number of characters can be in place of	the asterisk






Specific behaviours with functions:

- Functions that take an input file as an argument will only use the first result returned.


- If wildcard is used by itself it will attempt to run command for only the first result that is valid
	e.g. "cat *"
	- This order is based on the order in the directory
	- This only applies to certain commands. Chmod will apply to ALL files in the directory
	- ls will list all matching files



*/


/* Check if any of the arguments of a command contain wildcards and expand */
t_cmd * check_wildcard(t_cmd *cmd)
{
	int	i;

	i = 0;
	while (cmd->args[i])
	{
		if (is_set(cmd->args[i]))	/* Check if a wildcard exists in string */
			//Expand wildcard and return array of strings.
			//Insert the array of strings into the original args list at current point
		i++;
	}

}

/* If a wildcard is found, expand and return an array of strings containing all results */
char **expand_wildcard(char *arg)
{
	//Start at back of string and check if there are any forward slashes
	int	i;

	i = 0;


	//Seperate and directory movement information from search string, and use to access proper directory
	//Save this to add back on to results after search

	//Use ft_split to split string into relevant tokens, each token is a section of characters to search in order
	//Iterate through current directory and check each item, if there is a match add it to an array
	//Return array of matches






	/* test*ing*this

	This would match "test" to first 4 chars, "ing" to any characters outside of the first/last 4 chars, and "this" to last 4 chars
	The shortest possible match to any wildcard search is the exact string without the wildcard characters

	Search can be done from left to right. If any of the tokens fail to find a match, search can stop immediately



	*/
}