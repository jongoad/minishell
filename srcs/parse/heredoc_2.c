/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iyahoui- <iyahoui-@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/30 15:08:05 by jgoad             #+#    #+#             */
/*   Updated: 2022/08/05 15:19:47 by iyahoui-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_heredoc(int signum)
{
	t_shell	*sh;

	sh = get_data();
	cleanup(sh, false);
	(void)signum;
	exit(0);
}

char	*expand_heredoc_tok(char **envp, char **ptr)
{
	char	*var_name;
	char	*token;
	int		tok_len;
	int		i;

	token = NULL;
	if (**ptr != '$')
	{
		tok_len = get_tok_len(*ptr, "$");
		token = ft_xalloc((tok_len + 1) * sizeof(char));
		token = ft_strncpy(token, *ptr, tok_len);
		return (*ptr += tok_len, token);
	}
	*ptr += 1;
	tok_len = 0;
	while (ft_isalnum((*ptr)[tok_len]) || (*ptr)[tok_len] == '_')
		tok_len++;
	var_name = ft_xalloc((tok_len + 1) * sizeof(char));
	i = -1;
	while (++i < tok_len)
		var_name[i] = (*ptr)[i];
	var_name[i] = 0;
	token = expand_env_var(envp, var_name);
	free(var_name);
	return (*ptr += tok_len, token);
}

void	expand_heredoc(t_cmd *cmd, t_infile *in, char *heredoc)
{
	char	*token;
	char	*ptr;

	ptr = heredoc;
	while (ptr && *ptr)
	{
		token = expand_heredoc_tok(cmd->envp, &ptr);
		write(in->fd, token, ft_strlen(token));
		free(token);
	}
}
