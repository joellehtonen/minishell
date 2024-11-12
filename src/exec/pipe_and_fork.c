/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_and_fork.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlehtone <jlehtone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 10:35:14 by aklimchu          #+#    #+#             */
/*   Updated: 2024/11/12 12:52:32 by jlehtone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	pipe_and_fork(t_shell *shell, int loop_count)
{
	t_exec	*exec;

	exec = shell->exec;
	exec->pipe_flag = 0;
	if (loop_count < exec->pipe_num)
	{	
		if (pipe(exec->pipe[loop_count]) == -1)
		{
			close_pipes_parent(&exec);
			error_printer(shell, "", PIPE_FAIL, true);
		}
		exec->pipe_flag = 1;
	}
	exec->pid[loop_count] = fork();
	if (exec->pid[loop_count] == -1)
	{
		close_pipes_parent(&exec);
		error_printer(shell, "", FORK_FAIL, true);
	}
	if (exec->pid[loop_count] == 0)
		child_process(&shell, loop_count);
}
