/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklimchu <aklimchu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 14:43:42 by aklimchu          #+#    #+#             */
/*   Updated: 2024/10/31 10:09:53 by aklimchu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	check_for_here_doc(t_shell *shell, t_token *token, int loop_count);

static void	get_input_fd(t_shell **shell, int loop_count, int here_doc_index);

static void	get_output_fd(t_shell **shell, int loop_count);

void get_input_and_output(t_shell **shell, int loop_count)
{
	int		here_doc_index;

	check_all_files((*shell)->token_pointer, loop_count);
	here_doc_index = check_for_here_doc(*shell, (*shell)->token_pointer, loop_count);
	get_input_fd(shell, loop_count, here_doc_index);
	get_output_fd(shell, loop_count);
}

static int	check_for_here_doc(t_shell *shell, t_token *token, int loop_count)
{
	t_token	*temp;
	t_token	*res;
	int		here_doc_index;
	
	temp = token;
	res = NULL;
	while (temp && temp->level != loop_count)
		temp = temp->next;
	while (temp && temp->next && temp->level == loop_count)
	{
		if (temp->type == REDIR_INPUT)
		{
			res = temp;
			if (ft_strlen(res->line) == 1)
				check_file_access(shell, res->next->line, loop_count);
		}
		temp = temp->next;
	}
	if (!res || ft_strncmp(res->line, "<<\0", 3))
		return (-2);

	here_doc_index = 0;
	temp = token;
	while (temp && temp->next && temp->level <= loop_count)
	{
		if (temp->type == REDIR_INPUT && ft_strncmp(temp->line, "<<\0", 3) == 0)
			here_doc_index++;
		temp = temp->next;
	}
	return (here_doc_index - 1);
}

static void	get_input_fd(t_shell **shell, int loop_count, int here_doc_index)
{
	t_exec	*exec;
	
	exec = (*shell)->exec;
	if (here_doc_index >= 0)
	{
		if (dup2(exec->here_doc_pipe[here_doc_index][0], 0) == -1)
		{
			close_pipes_child(loop_count, &exec); // free pids?
			error_printer(*shell, DUP2_ERROR, true);
		}
	}
	else if (check_for_input(*shell, (*shell)->token_pointer, loop_count, 0) == 0)
	{
		if (dup2(exec->in, 0) == -1)
		{
			close_pipes_child(loop_count, &exec); // free pids?
			close(exec->in);
			error_printer(*shell, DUP2_ERROR, true);
		}
		close(exec->in);
	}
	else if (loop_count > 0 && dup2(exec->pipe[loop_count - 1][0], 0) == -1)
	{
		close_pipes_child(loop_count, &exec); // free pids?
		error_printer(*shell, DUP2_ERROR, true);
	}
}

static void	get_output_fd(t_shell **shell, int loop_count)
{
	t_exec	*exec;
	
	exec = (*shell)->exec;
	if (check_for_output(*shell, (*shell)->token_pointer, loop_count, 0) == 0)
	{
		if (dup2(exec->out, 1) == -1)
		{
			close_pipes_child(loop_count, &exec); // free pids?
			close(exec->out);
			error_printer(*shell, DUP2_ERROR, true);
		}
		close(exec->out);
	}
	else if (exec->pipe_flag == 1)
	{
		if (dup2(exec->pipe[loop_count][1], 1) == -1)
		{
			close_pipes_child(loop_count, &exec); // free pids?
			error_printer(*shell, DUP2_ERROR, true);
		}
		exec->pipe_flag = 0;
	}
}