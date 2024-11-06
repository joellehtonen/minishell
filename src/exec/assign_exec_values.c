/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assign_exec_values.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklimchu <aklimchu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 11:05:22 by aklimchu          #+#    #+#             */
/*   Updated: 2024/10/31 10:23:58 by aklimchu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	allocate_pipes(t_exec *exec, t_shell *shell);

void	assign_exec_values(t_shell *shell)
{
	t_exec *exec;
	
	exec = shell->exec;
	if (exec->pipe_num > 0)
		allocate_pipes(exec, shell);
	exec->null = NULL;
	exec->pid = (pid_t *)malloc((exec->pipe_num + 1) * sizeof(pid_t));
	if (exec->pid == NULL)
	    error_printer(shell, "", MALLOC_FAIL, true);
}

static void	allocate_pipes(t_exec *exec, t_shell *shell)
{
	int		i;
	
	exec->pipe = malloc(sizeof(int *) * exec->pipe_num);  // Allocate an array of pointers to pipes
   	if (!exec->pipe)
    	error_printer(shell, "", MALLOC_FAIL, true);
    // Allocate each pipe (each pipe is an array of 2 integers)
	i = 0;
    while (i < exec->pipe_num)
    {
    	exec->pipe[i] = malloc(sizeof(int) * 2);
       	if (!exec->pipe[i])
			error_printer(shell, "", MALLOC_FAIL, true);
		i++;
	}
}

void	allocate_here_doc(t_exec *exec, t_shell *shell)
{
	int		i;
	
	if (exec->here_doc_num <= 0)
		return ;
	exec->here_doc_pipe = malloc(sizeof(int *) * exec->here_doc_num);  // Allocate an array of pointers to pipes
   	if (!exec->here_doc_pipe)
		error_printer(shell, "", MALLOC_FAIL, true);
    // Allocate each pipe (each pipe is an array of 2 integers)
	i = 0;
    while (i < exec->here_doc_num)
    {
    	exec->here_doc_pipe[i] = malloc(sizeof(int) * 2);
       	if (!exec->here_doc_pipe[i])
			error_printer(shell, "", MALLOC_FAIL, true);
		i++;
	}
}