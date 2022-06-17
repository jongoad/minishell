/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enums_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iyahoui- <iyahoui-@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/13 19:22:23 by iyahoui-          #+#    #+#             */
/*   Updated: 2022/06/16 22:00:34 by iyahoui-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENUMS_BONUS_H
# define ENUMS_BONUS_H

/* Parse defines */
# define CL_SPEC_CH "\"\'$="
# define CL_SAME_TOK "\"\'$/="
# define CL_SAME_TOK_BONUS "\"\'$/=*"
# define WHITESPACES " \n\t\v\r\f"
# define CL_TOK_LIM " \n\t\v\r\f<>|$\"\'"
# define CL_VAR_LIM " \n\t\v\r\f<>|\"\'"
# define CL_DQU_LIM " \n\t\v\r\f<>|$\'"
# define CL_SQU_LIM " \n\t\v\r\f<>|$\""
# define CL_DQU_NOSPEC "$\""
# define ENV_VAR_CHARS "_abcdefghijklmnopqrstuvwxyz\
	ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"
# define CL_TOK_LIM_BONUS "* \n\t\v\r\f<>|$\"\'"
# define CL_SPEC_CH_BONUS "*\"\'$"

# define HEREDOC_PATH "/tmp/heredoc"
# define HIST_LOG ".minishell_history"
# define WILDCARD 26

# ifndef BONUS
#  define BONUS 0
# endif

/* validate.c flags */
typedef enum e_validation_states
{
	VALID = 0,
	EMPTY = 1,
	REDIR = 2
}	t_validation_states;

/* Builtin command flags */
typedef enum e_builtin
{
	E_ECHO = 0,
	E_CD = 1,
	E_PWD = 2,
	E_EXPORT = 3,
	E_UNSET = 4,
	E_ENV = 5,
	E_EXIT = 6
}	t_builtin_enum;

#endif