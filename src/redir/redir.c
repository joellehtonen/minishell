/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklimchu <aklimchu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 14:43:42 by aklimchu          #+#    #+#             */
/*   Updated: 2024/11/19 13:22:17 by aklimchu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	get_input_fd(t_shell **shell, int loop, int here_doc);
static int	get_output_fd(t_shell **shell, int loop);
static int	error_input(t_shell **shell, int loop, t_exec *exec);
static int	dup2_fd(int fd_1, int fd_2, int loop, t_shell **shell);

// The function checks the redirects relevant to current children process
// and gets the file descriptors of input and output files
int	get_input_and_output(t_shell **shell, int loop)
{
	int		here_doc_index;

	check_all_files((*shell)->token_pointer, (*shell)->exec, loop, *shell);
	here_doc_index = check_for_here_doc(*shell, (*shell)->token_pointer, loop);
	if (here_doc_index == -3)
		return (1);
	if (get_input_fd(shell, loop, here_doc_index) == 1)
		return (1);
	if (get_output_fd(shell, loop) == 1)
		return (1);
	return (0);
}

// The function goes through input redirects
// and duplicates the needed file descriptors to STD_IN
static int	get_input_fd(t_shell **shell, int loop, int here_doc)
{
	t_exec	*exec;

	exec = (*shell)->exec;
	if (here_doc >= 0 && exec->here_doc_closed == true)
		return (1);
	else if (here_doc >= 0)
	{
		if (dup2_fd(exec->here_doc_pipe[here_doc][0], 0, loop, shell) == 1)
			return (1);
	}
	else if (check_for_input(*shell, (*shell)->token_pointer, loop, 0) == 0)
	{
		if (dup2_fd(exec->in, 0, loop, shell) == 1)
			return (1);
		close(exec->in);
	}
	else if (check_for_input(*shell, (*shell)->token_pointer, loop, 0) == 1 \
		&& (*shell)->only_one_builtin == 1)
		return (1);
	else if (loop > 0 && dup2(exec->pipe[loop - 1][0], 0) == -1)
		return (error_input(shell, loop, exec));
	return (0);
}

// The function goes through input redirects
// and duplicates the needed file descriptors to STD_OUT
static int	get_output_fd(t_shell **shell, int loop)
{
	t_exec	*exec;

	exec = (*shell)->exec;
	if (check_for_output(*shell, (*shell)->token_pointer, loop, 0) == 0)
	{
		if (dup2_fd(exec->out, 1, loop, shell) == 1)
			return (1);
		close(exec->out);
	}
	else if (check_for_output(*shell, (*shell)->token_pointer, loop, 0) == 1 \
		&& (*shell)->only_one_builtin == 1)
		return (1);
	else if (exec->pipe_flag == 1)
	{
		if (dup2_fd(exec->pipe[loop][1], 1, loop, shell) == 1)
			return (1);
		exec->pipe_flag = 0;
	}
	return (0);
}

// Closing the pipes and exiting with error message related to DUP2 fail
static int	error_input(t_shell **shell, int loop, t_exec *exec)
{
	close_pipes_child(loop, &exec);
	error_printer(*shell, "", DUP2_ERROR, true);
	return (1);
}

// The function duplicated a file descriptor using dup2
static int	dup2_fd(int fd_1, int fd_2, int loop, t_shell **shell)
{
	if (dup2(fd_1, fd_2) == -1)
	{
		close_pipes_child(loop, &(*shell)->exec);
		error_printer(*shell, "", DUP2_ERROR, true);
		return (1);
	}
	return (0);
}
