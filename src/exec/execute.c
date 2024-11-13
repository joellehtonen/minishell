/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlehtone <jlehtone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 11:22:41 by aklimchu          #+#    #+#             */
/*   Updated: 2024/11/13 17:10:11 by jlehtone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	only_one_builtin(t_shell *shell);
static void	waiting_for_pids(t_exec *exec, int count, t_shell *shell);
void		close_pipes_parent(t_exec **exec);

int	execute(t_shell *shell)
{
	t_exec	*exec;
	int		loop_count;
	int		exit_status;

	exec = shell->exec;
	allocate_here_doc(exec, shell);
	if (exec->here_doc_num > 0 && here_doc(shell) == 1)
		return (1);
	if (exec->pipe_num == 0 && if_builtin(shell, 0) == 0)
		return (only_one_builtin(shell));
	assign_exec_values(shell);
	loop_count = 0;
	while (loop_count < exec->pipe_num + 1)
	{
		pipe_and_fork(shell, loop_count);
		loop_count++;
	}
	close_pipes_parent(&exec);
	waiting_for_pids(exec, loop_count - 1, shell);
	exit_status = exec->status;
	if (WIFEXITED(exit_status))
		return (WEXITSTATUS(exit_status));
	return (0);
}

static int	only_one_builtin(t_shell *shell)
{
	int		exit_status;

	shell->only_one_builtin = 1;
	shell->exec->orig_in = dup(STDIN_FILENO);
	shell->exec->orig_out = dup(STDOUT_FILENO);
	if (get_input_and_output(&shell, 0) == 1)
	{
		dup2(shell->exec->orig_in, STDIN_FILENO);
		dup2(shell->exec->orig_out, STDOUT_FILENO);
		close(shell->exec->orig_in);
		close(shell->exec->orig_out);
		return (1);
	}
	exit_status = exec_builtins(shell, 0);
	dup2(shell->exec->orig_in, STDIN_FILENO);
	dup2(shell->exec->orig_out, STDOUT_FILENO);
	close(shell->exec->orig_in);
	close(shell->exec->orig_out);
	return (exit_status);
}

static void	waiting_for_pids(t_exec *exec, int count, t_shell *shell)
{
	int	wait_error;

	shell->in_child = true;
	printf("in child status is %d\n", shell->in_child);
	set_up_signals(shell);
	wait_error = waitpid(exec->pid[count], &exec->status, 0);
	if (wait_error == -1 && errno == EINTR)
		return ;
	else if (wait_error == -1)
		error_printer(shell, "", WAITPID_ERROR, true);
	count = 0;
	printf("count is %d\n", count);
	while (count < exec->pipe_num)
	{
		if (waitpid(exec->pid[count], NULL, 0) == -1)
			error_printer(shell, "", WAITPID_ERROR, true);
		count++;
		printf("count is %d\n", count);
	}
	shell->in_child = false;
	printf("in child status is %d\n", shell->in_child);
	set_up_signals(shell);
}

void	close_pipes_parent(t_exec **exec)
{
	int		i;

	i = 0;
	while (i < (*exec)->pipe_num)
	{
		close((*exec)->pipe[i][0]);
		close((*exec)->pipe[i++][1]);
	}
	i = 0;
	while (i < (*exec)->here_doc_num)
		close((*exec)->here_doc_pipe[i++][0]);
}
