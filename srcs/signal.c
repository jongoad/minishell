/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iyahoui- <iyahoui-@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/13 15:56:00 by jgoad             #+#    #+#             */
/*   Updated: 2022/06/14 14:34:48 by iyahoui-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	void_sig(int sig)
{
	(void) sig;
}

/* Signal handler function */
void	signal_handler(int signum)
{
	t_shell	*sh;

	sh = get_data();
	if (signum == SIGINT)
	{
		printf("\n");
		rl_replace_line("", 1);
		rl_on_new_line();
		if (!sh->cmds)
		{
			rl_redisplay();
			sh->ret_val = 1;
		}
	}
	// else if (signum == SIGQUIT)
	// {
	// 	if (sh->cmds)
	// 	{
	// 		printf("Quit: %d\n", signum);
	// 		rl_replace_line("", 1);
	// 	}
	// 	rl_on_new_line();
	// 	if (!sh->cmds)
	// 		rl_redisplay();
	// }
}	
