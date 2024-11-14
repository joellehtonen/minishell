/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklimchu <aklimchu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 12:57:29 by jlehtone          #+#    #+#             */
/*   Updated: 2024/11/13 14:51:48 by aklimchu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// The function executes "env" builtin
int	env_exec(t_envp *envp, t_shell *shell, t_token *env, int loop)
{
	t_token	*arg;

	arg = find_token(env, loop, ARG);
	if (arg)
	{
		error_printer(shell, arg->line, NO_FILE_DIR_COMM, true);
		return (127);
	}
	while (envp)
	{
		printf("%s\n", envp->line);
		envp = envp->next;
	}
	free_and_exit(shell, 0);
	return (0);
}
