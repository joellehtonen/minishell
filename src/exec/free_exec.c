//42 header

#include "../../inc/minishell.h"

int	free_exec(t_exec **exec)
{
	int		i;
	
	if ((*exec)->pid)
	{
		free((*exec)->pid);
		(*exec)->pid = NULL;
	}
	i = 0;
	if ((*exec)->pipe)
    {
        // Iterate over each pipe and free its resources
        while (i < (*exec)->pipe_num)
        {
            if ((*exec)->pipe[i])  // Ensure the pipe pointer is not NULL
            {
                free((*exec)->pipe[i]);  // Free the individual pipe (array of 2 ints)
                (*exec)->pipe[i] = NULL;  // Avoid dangling pointer
            }
			i++;
        }
        // Free the array of pipe pointers
    	free((*exec)->pipe);
    	(*exec)->pipe = NULL;
	}
	if (*exec)
	{
		free(*exec);
		*exec = NULL;
	}
	return(1);
}

int	close_free(int pipe_flag, int fd1, int fd2, pid_t **pid)
{
	
	/* if (*pid)
	{
		free(*pid);
		*pid = NULL;
	} */
	if (pipe_flag  == 0)
		return (1);
	if (fd1 >= 0)
		close(fd1);
	if (fd2 >= 0)
		close(fd2);
	(void)pid;
	return (1);
}

int	free_all(char **arr_1, char **arr_2, char *str, pid_t **pid)
{
	if (arr_1)
		free_double_arr(arr_1);
	if (arr_2)
		free_double_arr(arr_2);
	if (str)
	{
		free(str);
		str = NULL;
	}
	if (*pid)
	{
		free(*pid);
		*pid = NULL;
	}
	return (1);
}

void	close_pipes_child(int loop_count, t_exec **exec)
{
	int		i;

	i = 0;
	while (i < (*exec)->pipe_num && i < loop_count + 1)
	{
		close((*exec)->pipe[i][0]);
		close((*exec)->pipe[i][1]);
		i++;
	}
}