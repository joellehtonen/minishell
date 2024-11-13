/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_and_fork.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklimchu <aklimchu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 10:35:14 by aklimchu          #+#    #+#             */
/*   Updated: 2024/11/13 11:09:49 by aklimchu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// The function creates a pipe and forks the process
void	pipe_and_fork(t_shell *shell, int loop)
{
	t_exec	*exec;

	exec = shell->exec;
	exec->pipe_flag = 0;
	if (loop < exec->pipe_num)
	{	
		if (pipe(exec->pipe[loop]) == -1)
		{
			close_pipes_parent(&exec);
			error_printer(shell, "", PIPE_FAIL, true);
		}
		exec->pipe_flag = 1;
	}
	exec->pid[loop] = fork();
	if (exec->pid[loop] == -1)
	{
		close_pipes_parent(&exec);
		error_printer(shell, "", FORK_FAIL, true);
	}
	if (exec->pid[loop] == 0)
		child_process(&shell, loop);
}
