//42 header

#include "../../inc/minishell.h"

int	free_exec(t_exec **exec)
{
	int		i;
	
	if (!*exec)
		return (1);
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
	//if (exec->here_doc_num <= 0)
	// free array with here_doc pipes?
	return(1);
}

int	free_all(char **arr_1, char **arr_2, char *str)
{
	if (arr_1)
		free_double_arr(&arr_1);
	if (arr_2)
		free_double_arr(&arr_2);
	if (str)
		free_str(&str);
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
	i = 0;
	while (i < (*exec)->here_doc_num)
	{
		close((*exec)->here_doc_pipe[i][0]);
		//close((*exec)->here_doc[i][1]);
		i++;
	}
}