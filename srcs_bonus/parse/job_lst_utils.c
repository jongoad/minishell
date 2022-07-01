/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job_lst_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iyahoui- <iyahoui-@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 13:26:51 by iyahoui-          #+#    #+#             */
/*   Updated: 2022/07/01 13:27:17 by iyahoui-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

void	ms_jobadd(t_ms_job **lst, t_ms_job *new)
{
	t_ms_job	*ptr;

	if (!lst)
		return ;
	if (*lst)
	{
		ptr = *lst;
		while (ptr->next)
			ptr = ptr->next;
		ptr->next = new;
	}
	else
		*lst = new;
}

t_ms_job	*ms_jobnew(t_cmd **cmds, int nb_cmds, char operator)
{
	t_ms_job	*new_node;

	if (!cmds)
		return (NULL);
	new_node = ft_xalloc(sizeof(t_ms_job));
	if (!new_node)
		return (NULL);
	new_node->cmds = cmds;
	new_node->operator = operator;
	new_node->nb_cmds = nb_cmds;
	new_node->next = NULL;
	return (new_node);
}

void	ms_jobclear(t_ms_job **lst)
{
	t_ms_job	*ptr;
	t_ms_job	*buffer;

	if (!lst && !*lst)
		return ;
	ptr = *lst;
	while (ptr)
	{
		buffer = ptr->next;
		ms_jobdelone(ptr);
		ptr = buffer;
	}
	*lst = NULL;
}

void	ms_jobdelone(t_ms_job *lst)
{
	if (!lst)
		return ;
	free (lst);
}

t_ms_job	*ms_joblast(t_ms_job *lst)
{
	t_ms_job	*ptr;

	if (!lst)
		return (lst);
	ptr = lst;
	while (ptr->next)
		ptr = ptr->next;
	return (ptr);
}
