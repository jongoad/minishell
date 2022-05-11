#include "../includes/minishell.h"

/* Convert a value held in a string to an int and check if valid */
long long	ft_atoll(const char *str, bool *is_valid)
{
	long long	num;
	long long	sign;
	long long	prev;

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
		prev = num;
		num = (num * 10) + (*str - '0');
		str++;
		if (abs_val(num * sign) < prev)
			*is_valid = false;
	}
	return (num * sign);
}

/* Add a new environment variable to the envp array */
void	add_env_var(t_env *env, char *str)
{
	int	i;
	char **tmp;

	i = count_array((void **)env->envp);
	tmp = (char **)malloc(sizeof(char *) * i + 2);
	i = 0;
	while (env->envp[i])
	{
		tmp[i] = ft_strdup(env->envp[i]);
		i++;
	}
	tmp[i] = ft_strdup(str);
	tmp[i + 1] = NULL;
	i = -1;
	while (env->envp[++i])
		free(env->envp[i]);
	free(env->envp);
	env->envp = tmp;
}
/* Change value of an environment variable */
int	change_env_var(t_env *env, char *arg)
{
	int	i;
	char **split;

	i = 0;
	split = ft_split(arg, '=');
	while (env->envp[i])
	{
		if (!ft_strncmp(split[0], env->envp[i], ft_strlen(split[0])) &&
			env->envp[i][ft_strlen(split[0])] == '=')
		{
			free(env->envp[i]);
			env->envp[i] = ft_strjoin(ft_strjoin(split[0], "="), split[1]);
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
	int i;
	int j;
	char **tmp;

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
	i = -1;
	while (env->envp[++i])
		free(env->envp[i]);
	free(env->envp);
	env->envp = tmp;
}

/* Check if environment variable name is valid */
bool check_env_var(char *str, bool unset)
{
	int	i;

	i = 0;
	if (!str[i] || (str[i] >= '0' && str[i] <= '9') || (str[i] == '_' && str[i + 1] == '\0'))		/* If null string or if variable name is a single underscore or a number, return invalid */
		return (false);
	while (str[i] && str[i] != '=')					/* Iterate until equals sign is hit */
	{
		if (!(str[i] >= '0' && str[i] <= '9')
			|| !(str[i] >= 'a' && str[i] <= 'z')
			|| !(str[i] >= 'A' && str[i] <= 'Z') || str[i] != '_')
			return (false);
		i++;
	}
	if (!unset && str[i] != '=')								/* If no equals sign is hit, return false */
		return (false);
	return (true);
}

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
	ft_memset((void*)buf, 0, 1025);
	buf = getcwd(buf, 1025);
	tmp = ft_strdup(buf);
	free(buf);
	return (tmp);
}
//FIX check error management
