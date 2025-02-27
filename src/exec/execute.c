/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklimchu <aklimchu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 11:22:41 by aklimchu          #+#    #+#             */
/*   Updated: 2024/11/22 11:24:48 by aklimchu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	only_one_builtin(t_shell *shell);
static void	waiting_for_pids(t_exec *exec, int count, t_shell *shell);
static int	child_loop(t_shell *shell, t_exec *exec);
static void	wait_loop(t_exec *exec, t_shell *shell);

// The function executes the commands passed by user
int	execute(t_shell *shell)
{
	t_exec	*exec;
	int		loop_count;
	int		exit_status;
	int		here_doc_return;

	exec = shell->exec;
	allocate_here_doc(exec, shell);
	if (exec->here_doc_num > 0)
	{
		here_doc_return = here_doc(shell);
		if (here_doc_return == 1)
			return (1);
		if (here_doc_return == 130)
			return (130);
	}
	if (exec->pipe_num == 0 && if_builtin(shell, 0) == 0)
		return (only_one_builtin(shell));
	loop_count = child_loop(shell, exec);
	waiting_for_pids(exec, loop_count - 1, shell);
	exit_status = exec->status;
	if (shell->exit_code == 131)
		return (131);
	if (WIFEXITED(exit_status))
		return (WEXITSTATUS(exit_status));
	return (0);
}

// this executes if there is only command, no pipes,
// and the command is a builtin
static int	only_one_builtin(t_shell *shell)
{
	int		exit_status;

	shell->only_one_builtin = 1;
	shell->exec->orig_in = dup(STDIN_FILENO);
	shell->exec->orig_out = dup(STDOUT_FILENO);
	if (shell->exec->orig_in == -1 || shell->exec->orig_out == -1)
		error_printer(shell, "", DUP2_ERROR, true);
	if (get_input_and_output(&shell, 0) == 1)
	{
		if (dup2(shell->exec->orig_in, STDIN_FILENO) == -1)
			error_printer(shell, "", DUP2_ERROR, true);
		if (dup2(shell->exec->orig_out, STDOUT_FILENO) == -1)
			error_printer(shell, "", DUP2_ERROR, true);
		close(shell->exec->orig_in);
		close(shell->exec->orig_out);
		return (1);
	}
	exit_status = exec_builtins(shell, 0);
	if (dup2(shell->exec->orig_in, STDIN_FILENO) == -1)
		error_printer(shell, "", DUP2_ERROR, true);
	if (dup2(shell->exec->orig_out, STDOUT_FILENO) == -1)
		error_printer(shell, "", DUP2_ERROR, true);
	close(shell->exec->orig_in);
	close(shell->exec->orig_out);
	return (exit_status);
}

static int	child_loop(t_shell *shell, t_exec *exec)
{
	int	loop_count;

	assign_exec_values(shell);
	loop_count = 0;
	while (loop_count < exec->pipe_num + 1)
	{
		pipe_and_fork(shell, loop_count);
		loop_count++;
	}
	close_pipes_parent(&exec);
	return (loop_count);
}

// The function waits for all children processes to finish
static void	waiting_for_pids(t_exec *exec, int count, t_shell *shell)
{
	int	wait_error;

	shell->in_child = true;
	set_up_signals(shell);
	wait_error = waitpid(exec->pid[count], &exec->status, 0);
	shell->child_exit_code = exec->status;
	if (wait_error == -1 && errno == EINTR)
	{
		shell->in_child = false;
		set_up_signals(shell);
		shell->exit_code = 131;
		return ;
	}
	else if (wait_error == -1)
		error_printer(shell, "", WAITPID_ERROR, true);
	wait_loop(exec, shell);
	shell->in_child = false;
	set_up_signals(shell);
}

static void	wait_loop(t_exec *exec, t_shell *shell)
{
	int	count;
	int	exit_process;

	count = 0;
	exit_process = -1;
	while (count < exec->pipe_num)
	{
		if (waitpid(exec->pid[count], &exit_process, 0) == -1)
			error_printer(shell, "", WAITPID_ERROR, true);
		if (exit_process == 0)
			shell->child_exit_code = 0;
		count++;
	}
}
