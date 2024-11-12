/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlehtone <jlehtone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 12:57:29 by jlehtone          #+#    #+#             */
/*   Updated: 2024/11/12 12:57:51 by jlehtone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

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
