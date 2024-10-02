//42 header

#include "../../inc/minishell.h"

int	free_exec(t_exec **exec)
{
	if ((*exec)->pid)
	{
		free((*exec)->pid);
		(*exec)->pid = NULL;
	}
	free(*exec);
	return(1);
}

int	close_free(int pipe_flag, int fd1, int fd2, pid_t **pid)
{
	
	if (*pid)
	{
		free(*pid);
		*pid = NULL;
	}
	if (pipe_flag  == 0)
		return (1);
	if (fd1 >= 0)
		close(fd1);
	if (fd2 >= 0)
		close(fd2);
	return (1);
}