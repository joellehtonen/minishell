/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_and_fork.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklimchu <aklimchu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 10:35:14 by aklimchu          #+#    #+#             */
/*   Updated: 2024/11/12 15:14:45 by aklimchu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

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
	/* if (find_token_line(shell->token_pointer, loop, COMM, "./minishell")) //&& check if we can execute?
	{
		if (change_shlvl(&(shell->envp_copy), 1, shell) == 1))
		return ;
	} */
	if (exec->pid[loop] == 0)
		child_process(&shell, loop);
	/* if (find_token_line(shell->token_pointer, loop, COMM, "./minishell"))
		change_shlvl(&(shell->envp_copy), -1, shell); */
}
