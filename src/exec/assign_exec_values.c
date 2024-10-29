/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assign_exec_values.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklimchu <aklimchu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 11:05:22 by aklimchu          #+#    #+#             */
/*   Updated: 2024/10/29 13:43:46 by aklimchu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	allocate_pipes(t_exec *exec);

void	allocate_here_doc(t_exec *exec);

int	assign_exec_values(t_shell *shell)
{
	t_exec *exec;
	
	exec = shell->exec;
	if (exec->pipe_num > 0)
		allocate_pipes(exec);
	exec->null = NULL;
	printf("pipe_num %d\n", exec->pipe_num);
	exec->pid = (pid_t *)malloc((exec->pipe_num + 1) * sizeof(pid_t));
	if (exec->pid == NULL)
	{
		perror("malloc() failed");
		return (1);
	}
	return (0);
}

static void	allocate_pipes(t_exec *exec)
{
	int		i;
	
	exec->pipe = malloc(sizeof(int *) * exec->pipe_num);  // Allocate an array of pointers to pipes
   	if (!exec->pipe)
    {
    	perror("malloc() failed");
       	exit(EXIT_FAILURE);
   	}
    // Allocate each pipe (each pipe is an array of 2 integers)
	i = 0;
    while (i < exec->pipe_num)
    {
    	exec->pipe[i] = malloc(sizeof(int) * 2);
       	if (!exec->pipe[i])
       	{
           	perror("malloc() failed");
           	exit(EXIT_FAILURE);
       	}
		i++;
	}
}

void	allocate_here_doc(t_exec *exec)
{
	int		i;
	
	if (exec->here_doc_num <= 0)
		return ;
	exec->here_doc_pipe = malloc(sizeof(int *) * exec->here_doc_num);  // Allocate an array of pointers to pipes
   	if (!exec->here_doc_pipe)
    {
    	perror("malloc() failed");
       	exit(EXIT_FAILURE);
   	}
    // Allocate each pipe (each pipe is an array of 2 integers)
	i = 0;
    while (i < exec->here_doc_num)
    {
    	exec->here_doc_pipe[i] = malloc(sizeof(int) * 2);
       	if (!exec->here_doc_pipe[i])
       	{
           	perror("malloc() failed");
           	exit(EXIT_FAILURE);
       	}
		i++;
	}
}