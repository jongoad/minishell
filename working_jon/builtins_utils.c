#include "../includes/minishell.h"

/* Convert a value held in a string to an int and check if valid */
long long	atoll(const char *str, bool *is_valid)
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

/* Init builtin commands struct */
void	init_builtins(t_shell *sh)
{
	int	i;

	i = 0;
	/* Init alias strings */
	sh->builtins.alias[0] = "echo";
	sh->builtins.alias[1] = "cd";
	sh->builtins.alias[2] = "pwd";
	sh->builtins.alias[3] = "export";
	sh->builtins.alias[4] = "unset";
	sh->builtins.alias[5] = "env";
	sh->builtins.alias[6] = "exit";

	/*Init fucntion pointer array */
	sh->builtins.f[0] = &builtin_echo;
	sh->builtins.f[1] = &builtin_cd;
	sh->builtins.f[2] = &builtin_pwd;
	sh->builtins.f[3] = &builtin_export;
	sh->builtins.f[4] = &builtin_unset;
	sh->builtins.f[5] = &builtin_env;
	sh->builtins.f[6] = &builtin_exit;
}