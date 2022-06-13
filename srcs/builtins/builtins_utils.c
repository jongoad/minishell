/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iyahoui- <iyahoui-@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/13 17:02:49 by jgoad             #+#    #+#             */
/*   Updated: 2022/06/13 17:23:35 by iyahoui-         ###   ########.fr       */
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
	t_shell	*sh;

	sh = get_data();
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
	if (!ft_strncmp(str, "PATH=", 5))
		init_path(sh);
}

/* Change value of an environment variable */
int	change_env_var(t_env *env, char *arg)
{
	int		i;
	char	**split;
	char	*tmp;
	t_shell	*sh;

	i = -1;
	split = ft_split(arg, '=');
	sh = get_data();
	while (env->envp[++i])
	{
		if (!ft_strncmp(split[0], env->envp[i], ft_strlen(split[0]))
			&& env->envp[i][ft_strlen(split[0])] == '=')
		{
			free(env->envp[i]);
			tmp = ft_strjoin(split[0], "=");
			env->envp[i] = ft_strjoin(tmp, split[1]);
			free(tmp);
			if (!ft_strncmp(split[0], "PATH", ft_strlen(split[0])))
				init_path(sh);
			free_array((void **)split);
			return (1);
		}
	}
	free_array((void **)split);
	return (0);
}

/* Remove an env variable */
void	remove_env_var(t_env *env, int n)
{
	int		i;
	int		j;
	bool	unset_path;
	char	**tmp;
	t_shell	*sh;

	i = count_array((void **)env->envp);
	tmp = ft_xalloc(sizeof(char *) * i);
	sh = get_data();
	i = -1;
	j = 0;
	unset_path = false;
	while (env->envp[++i])
	{
		//MODIFIED
		if (i == n && !ft_strncmp(env->envp[i], "PATH=", 5))
			unset_path = true;
		else if (i != n)
			tmp[j++] = ft_strdup(env->envp[i]);
	}
	free_array((void **)env->envp);
	env->envp = tmp;
	if (unset_path)
		init_path(sh);
}

/* Check if environment variable name is valid */
bool	check_env_var(char *str, bool unset)
{
	int	i;

	i = 0;
	if (!str[i] || (str[i] >= '0' && str[i] <= '9')
		|| (str[i] == '_' && str[i + 1] == '\0'))
		return (false);
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (false);
		i++;
	}
	if (!unset && str[i] != '=')
		return (false);
	else if (unset && str[i] != '\0')
		return (false);
	return (true);
}
