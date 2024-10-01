//42 header

#include "../inc/minishell.h"

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

int	close_free(int fd1, int fd2, int fd3, pid_t **pid)
{
	if (fd1 >= 0)
		close(fd1);
	if (fd2 >= 0)
		close(fd2);
	if (fd3 >= 0)
		close(fd3);
	if (*pid)
	{
		free(*pid);
		*pid = NULL;
	}
	return (1);
}