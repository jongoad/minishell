/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iyahoui- <iyahoui-@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/13 16:48:00 by jgoad             #+#    #+#             */
/*   Updated: 2022/06/29 22:40:26 by iyahoui-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

/* Print error message and return errnum */
int	msg_err_ret(int errnum, char *errname)
{
	errno = errnum;
	putstr_fd("minishell: ", 2);
	perror(errname);
	return (errnum);
}

/* Print error message and return errnum */
int	fct_err_msg(char *fct, char *errname)
{
	printf("%s: %s", fct, errname);
	return (errno);
}

/* Write bash-style error messages to STDERR */
void	put_err_msg(char *sh_name, char *cmd, char *arg, char *msg)
{
	if (sh_name)
	{
		putstr_fd(sh_name, 2);
		putstr_fd(": ", 2);
	}
	if (cmd)
	{
		putstr_fd(cmd, 2);
		putstr_fd(": ", 2);
	}
	if (arg)
	{
		putstr_fd("'", 2);
		putstr_fd(arg, 2);
		putstr_fd("'", 2);
		putstr_fd(": ", 2);
	}
	if (msg)
		putstr_fd(msg, 2);
	putstr_fd("\n", 2);
}

/* Parse error printing function */
int	parse_error(char err_char)
{
	if (err_char)
	{
		printf("\033[38;5;9mminishell: ");
		if (err_char == '\n')
			printf("parse error near unexpected token `newline\'");
		else if (err_char == UNCLOSED_PARENTHESIS)
			printf("parse error: unclosed parenthesis");
		else if (err_char == EMPTY_ARG)
			printf("parse error: empty parenthesis");
		else if (err_char == -1)
			printf("parse error: empty token");
		else
			printf("parse error near unexpected token `%c\'", err_char);
		printf("\033[0m\n");
	}
	return (1);
}
