//42 header

#include "../../inc/minishell.h"

int	pipe_and_fork(t_shell *shell, int loop_count)
{
	t_exec	*exec;
	
	exec = shell->exec;
	exec->pipe_flag = 0;
	if (loop_count < exec->pipe_num)
	{	
		if (pipe(exec->pipe[loop_count]) == -1)
		{
			perror("Pipe failed");
			return (1);
		}
		exec->pipe_flag = 1;
	}
	exec->pid[loop_count] = fork();
	if (exec->pid[loop_count] == -1)
	{
		perror("Fork failed");
		return (1);
	}
	if (exec->pid[loop_count] == 0)
		child_process(&shell, loop_count);
	return (0);
}