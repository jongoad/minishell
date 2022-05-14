#include "minishell.h"

void	ms_lstadd(t_arglst **lst, t_arglst *new)
{
	t_arglst	*ptr;

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

t_arglst	*ms_lstnew(char *str, bool is_env_var)
{
	t_arglst	*new_node;

	if (!str)
		return (NULL);
	new_node = malloc(sizeof(t_arglst));
	if (!new_node)
		return (NULL);
	new_node->str = str;
	new_node->is_env_var = is_env_var;
	new_node->next = NULL;
	return (new_node);
}

void	ms_lstclear(t_arglst **lst)
{
	t_arglst	*ptr;
	t_arglst	*buffer;

	if (!lst)
		return ;
	ptr = *lst;
	while (ptr)
	{
		buffer = ptr->next;
		ms_lstdelone(ptr);
		ptr = buffer;
	}
	*lst = NULL;
}

void	ms_lstdelone(t_arglst *lst)
{
	if (!lst)
		return ;
	free (lst->str);
	free (lst);
}
