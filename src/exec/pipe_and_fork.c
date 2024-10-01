//42 header

#include "../inc/minishell.h"

int	pipe_and_fork(t_shell *shell, int i)
{
	t_exec	*exec;
	int		flag_pipe;
	
	exec = shell->exec;
	flag_pipe = 0;
	if (i < exec->pipe_num)
	{	
		if (pipe(exec->pipe) == -1)
		{
			perror("Pipe failed");
			return (close_free(1, exec->in, -1, &exec->null));
		}
		flag_pipe = 1;
	}
	exec->pid[i] = fork();
	if (exec->pid[i] == -1)
	{
		perror("Fork failed");
		return (close_free(flag_pipe, exec->pipe[0], exec->pipe[1], &exec->null));
	}
	if (exec->pid[i] == 0)
		child_process(&shell, i, flag_pipe);
	if (flag_pipe == 1 && dup2(exec->pipe[0], 0) == -1)
	{
		close_free(flag_pipe, exec->pipe[0], exec->pipe[1], &exec->null);
		perror("dup() error");
		exit(1);
	}
	close_free(flag_pipe, exec->pipe[0], exec->pipe[1], &exec->null);
	return (0);
}