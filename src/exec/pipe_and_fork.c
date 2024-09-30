//42 header

#include "../inc/minishell.h"

int	pipe_and_fork(t_shell *shell, int i)
{
	t_exec	*exec;
	int		flag_pipe;
	
	exec = &shell->exec;
	flag_pipe = 0;
	if (i < exec->pipe_num)
	{	
		if (pipe(exec->pipe) == -1)
		{
			perror("Pipe failed");
			return (1/* close_free(fd->in, -1, -1, &fd->null) */);
		}
		flag_pipe = 1;
	}
	exec->pid[i] = fork();
	if (exec->pid[i] == -1)
	{
		perror("Fork failed");
		return (1/* close_free(fd->in, fd->pipe[0], fd->pipe[1], &fd->null) */);
	}
	if (exec->pid[i] == 0)
		child_process(shell, i, flag_pipe);
	if (flag_pipe == 1 && dup2(exec->pipe[0], 0) == -1)
	{
		/* close_free(fd->in, fd->pipe[0], fd->pipe[1], &fd->null); */
		perror("dup() error");
		exit(1);
	}
	/* close_free(fd->in, fd->pipe[0], fd->pipe[1], &fd->null); */

	return (0);
}