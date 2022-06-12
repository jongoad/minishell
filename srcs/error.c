/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iyahoui- <iyahoui-@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/09 15:09:58 by jgoad             #+#    #+#             */
/*   Updated: 2022/06/12 16:34:22 by iyahoui-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	putstr_fd("minishell: syntax error near unexpected token `", 2);
	if (err_char == '\n')
		write(2, "newline", 7);
	else
		write(2, &err_char, 1);
	write(2, "\'\n", 2);
	return (1);
}
