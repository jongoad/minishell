/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iyahoui- <iyahoui-@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/09 14:49:38 by jgoad             #+#    #+#             */
/*   Updated: 2022/06/12 19:26:51 by iyahoui-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Convert a value held in a string to an int and check if valid */
long long	ft_atoll(const char *str, bool *is_valid)
{
	long long	num;
	long long	sign;

	num = 0;
	sign = 1;
	*is_valid = true;
	if (*str == '+' || *str == '-')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str)
	{	
		if (*str < '0' || *str > '9')
			*is_valid = false;
		num = (num * 10) + (*str - '0');
		str++;
		if ((num * sign < 0 && sign == 1) || (num * sign > 0 && sign == -1))
			*is_valid = false;
	}
	return (num * sign);
}

/* Add a new environment variable to the envp array */
void	add_env_var(t_env *env, char *str)
{
	int		i;
	char	**tmp;

	i = count_array((void **)env->envp);
	tmp = (char **)malloc(sizeof(char *) * (i + 2));
	i = 0;
	while (env->envp[i])
	{
		tmp[i] = ft_strdup(env->envp[i]);
		i++;
	}
	tmp[i] = ft_strdup(str);
	tmp[i + 1] = NULL;
	i = 0;
	while (env->envp[i])
	{
		free(env->envp[i]);
		i++;
	}
	free(env->envp);
	env->envp = tmp;
}

/* Change value of an environment variable */
int	change_env_var(t_env *env, char *arg)
{
	int		i;
	char	**split;
	char	*tmp;

	i = 0;
	split = ft_split(arg, '=');
	while (env->envp[i])
	{
		if (!ft_strncmp(split[0], env->envp[i], ft_strlen(split[0]))
			&& env->envp[i][ft_strlen(split[0])] == '=')
		{
			free(env->envp[i]);
			tmp = ft_strjoin(split[0], "=");
			env->envp[i] = ft_strjoin(tmp, split[1]);
			free(tmp);
			free_array((void **)split);
			return (1);
		}
		i++;
	}
	return (0);
}

/* Remove an env variable */
void	remove_env_var(t_env *env, int n)
{
	int		i;
	int		j;
	char	**tmp;

	i = count_array((void **)env->envp);
	tmp = (char **)malloc(sizeof(char *) * i);
	tmp[i] = NULL;
	i = 0;
	j = 0;
	while (env->envp[i])
	{
		if (i == n)
			i++;
		else
		{
			tmp[j] = ft_strdup(env->envp[i]);
			i++;
			j++;
		}
	}
	free_array((void **)env->envp);
	env->envp = tmp;
}

/* Check if environment variable name is valid */
bool	check_env_var(char *str, bool unset)
{
	int	i;

	i = 0;
	if (!str[i] || (str[i] >= '0' && str[i] <= '9')
		|| (str[i] == '_' && str[i + 1] == '\0'))					/* If null string or if variable name is a single underscore or a number, return invalid */
		return (false);
	while (str[i] && str[i] != '=')									/* Iterate until equals sign is hit */
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (false);
		i++;
	}
	if (!unset && str[i] != '=')									/* If no equals sign is hit, return false */
		return (false);
	else if (unset && str[i] != '\0')
		return (false);
	return (true);
}
