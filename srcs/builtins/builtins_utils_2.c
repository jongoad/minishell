/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iyahoui- <iyahoui-@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/09 14:48:54 by jgoad             #+#    #+#             */
/*   Updated: 2022/06/12 16:23:29 by iyahoui-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Compare env var name passed to unset to record in env var  array */
bool	env_var_cmp(char *arg_str, char *env_str)
{
	int	i;

	i = 0;
	while (arg_str[i] && env_str[i] && env_str[i] != '=')
	{
		if (arg_str[i] != env_str[i])
			return (false);
		i++;
	}
	if (!arg_str[i] && env_str[i] == '=')
		return (true);
	return (false);
}

/* Print path to current directory */
char	*pwd_to_str(void)
{
	char	*buf;
	char	*tmp;

	buf = (char *)malloc(sizeof(char) * 1025);
	ft_memset((void *)buf, 0, 1025);
	buf = getcwd(buf, 1025);
	tmp = ft_strdup(buf);
	free(buf);
	return (tmp);
}
