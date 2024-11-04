/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklimchu <aklimchu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 11:22:41 by aklimchu          #+#    #+#             */
/*   Updated: 2024/10/31 11:23:09 by aklimchu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	only_one_builtin(t_shell *shell);

static int	waiting_for_pids(t_exec *exec, int count);

static void	close_pipes_parent(t_exec **exec);

int	execute(t_shell *shell)
{
	t_exec	*exec;
	int		loop_count;
	int		exit_status;

	exec = shell->exec;
	allocate_here_doc(exec, shell);
	if (exec->here_doc_num > 0 && here_doc(shell) == 1)
		return (free_exec(&exec));
	if (exec->pipe_num == 0 && if_builtin(shell, 0) == 0)
		return (only_one_builtin(shell));
	if (assign_exec_values(shell) == 1)
		return (free_exec(&exec));
	loop_count = 0;
	while (loop_count < exec->pipe_num + 1)
	{
		if (pipe_and_fork(shell, loop_count) == 1)
		{
			close_pipes_parent(&exec);
			return (free_exec(&exec));
		}
		loop_count++;
	}
	close_pipes_parent(&exec);
	if (waiting_for_pids(exec, loop_count - 1) == 1)
		return (free_exec(&exec));
	exit_status = exec->status;
	free_exec(&exec);
	if (WIFEXITED(exit_status))
		return (WEXITSTATUS(exit_status));
	return (0);
}

static int	only_one_builtin(t_shell *shell)
{
	int		exit_status;
	int		orig_in;
	int		orig_out;

	shell->only_one_builtin = 1;
	orig_in = dup(STDIN_FILENO);
	orig_out = dup(STDOUT_FILENO);
	if (get_input_and_output(&shell, 0) == 1)
	{
		dup2(orig_in, STDIN_FILENO);
		dup2(orig_out, STDOUT_FILENO);
    	close(orig_in);
    	close(orig_out);
		return (1);
	}
	exit_status = exec_builtins(shell, 0);
	dup2(orig_in, STDIN_FILENO);
	dup2(orig_out, STDOUT_FILENO);
    close(orig_in);
    close(orig_out);
	return (exit_status);
}

static int	waiting_for_pids(t_exec *exec, int count)
{
	if (waitpid(exec->pid[count], &exec->status, 0) == -1)
	{
		perror("wait() error");
		return (1);
	}
	count = 0;
	while (count < exec->pipe_num)
	{
		if (waitpid(exec->pid[count], NULL, 0) == -1)
		{
			perror("wait() error");
			return (1);
		}
		count++;
	}
	return (0);
}

static void	close_pipes_parent(t_exec **exec)
{
	int		i;

	i = 0;
	while (i < (*exec)->pipe_num)
	{
		close((*exec)->pipe[i][0]);
		close((*exec)->pipe[i][1]);
		i++;
	}
	i = 0;
	while (i < (*exec)->here_doc_num)
	{
		close((*exec)->here_doc_pipe[i][0]);
		//close((*exec)->here_doc[i][1]);
		i++;
	}
}
