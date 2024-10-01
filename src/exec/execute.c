//42 header

#include "../inc/minishell.h"

static int	assign_exec_values(t_shell *shell);

static int	waiting_for_pids(t_exec *exec, int count);

int	execute(t_shell *shell)
{
	t_exec	*exec;
	int		loop_count;
	int		exit_status;

	exec = shell->exec;
	if (assign_exec_values(shell) == 1)
		return (free_exec(&shell->exec));
	loop_count = 0;
	while (loop_count < exec->pipe_num + 1)
	{
		if (pipe_and_fork(shell, loop_count) == 1)
			return (free_exec(&shell->exec));
		loop_count++;
	}
	if (waiting_for_pids(exec, loop_count - 1) == 1)
		return (free_exec(&shell->exec));
	exit_status = exec->status;
	free_exec(&shell->exec);
	if (WIFEXITED(exit_status))
		return (WEXITSTATUS(exit_status));
	return (0);
}

static int	assign_exec_values(t_shell *shell)
{
	t_exec *exec;

	exec = shell->exec;
	printf("pipe_num %d\n", exec->pipe_num);
	exec->pid = (pid_t *)malloc((exec->pipe_num + 1) * sizeof(pid_t));
	if (exec->pid == NULL)
	{
		perror("malloc() failed");
		return (1);
	}
	return (0);
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
