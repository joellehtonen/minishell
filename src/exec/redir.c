/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklimchu <aklimchu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 14:43:42 by aklimchu          #+#    #+#             */
/*   Updated: 2024/11/05 11:35:45 by aklimchu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	check_for_here_doc(t_shell *shell, t_token *token, int loop_count);

/* static int	check_error_node(t_shell *shell, int loop_count);
 */
static int	get_input_fd(t_shell **shell, int loop_count, int here_doc_index);

static int	get_output_fd(t_shell **shell, int loop_count);

int get_input_and_output(t_shell **shell, int loop_count)
{
	int		here_doc_index;

	check_all_files((*shell)->token_pointer, (*shell)->exec, loop_count, *shell);
	here_doc_index = check_for_here_doc(*shell, (*shell)->token_pointer, loop_count);
	if (here_doc_index == -3)
		return (1);
	if (get_input_fd(shell, loop_count, here_doc_index) == 1)
		return (1);
	if (get_output_fd(shell, loop_count) == 1)
		return (1);
	return (0);
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
	if (check_for_output_no_recur(shell, temp, loop_count, shell->exec->error_node_index) == 1)
		return (-3);
	temp = token;
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

/* static int	check_error_node(t_shell *shell, int loop_count)
{
	int		error_node;
	int		i;
	t_token *temp;

	error_node = shell->exec->error_node_index;
	if (error_node == -1)
		return (0);
	i = 0;
	temp = shell->token_pointer;
	while (temp && temp->level != loop_count)
		temp = temp->next;
	while (i < error_node)
	{
		if (temp->type == REDIR_OUTPUT)
			return (1);
	}
	return (0);
} */

static int	get_input_fd(t_shell **shell, int loop_count, int here_doc_index)
{
	t_exec	*exec;
	
	exec = (*shell)->exec;
	if (here_doc_index >= 0)
	{
		if (dup2(exec->here_doc_pipe[here_doc_index][0], 0) == -1)
		{
			close_pipes_child(loop_count, &exec); // free pids?
			error_printer(*shell, DUP2_ERROR, true);
			return (1);
		}
	}
	else if (check_for_input(*shell, (*shell)->token_pointer, loop_count, 0) == 0)
	{
		if (dup2(exec->in, 0) == -1)
		{
			close_pipes_child(loop_count, &exec); // free pids?
			close(exec->in);
			error_printer(*shell, DUP2_ERROR, true); // DUP2_ERROR
			return (1);
		}
		close(exec->in);
	}
	else if (check_for_input(*shell, (*shell)->token_pointer, loop_count, 0) == 1 &&\
		(*shell)->only_one_builtin == 1)
		return (1);
	else if (loop_count > 0 && dup2(exec->pipe[loop_count - 1][0], 0) == -1)
	{
		close_pipes_child(loop_count, &exec); // free pids?
		error_printer(*shell, DUP2_ERROR, true);
		return (1);
	}
	return (0);
}

static int	get_output_fd(t_shell **shell, int loop_count)
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
	else if (check_for_output(*shell, (*shell)->token_pointer, loop_count, 0) == 1 &&\
		(*shell)->only_one_builtin == 1)
		return (1);
	else if (exec->pipe_flag == 1)
	{
		if (dup2(exec->pipe[loop_count][1], 1) == -1)
		{
			close_pipes_child(loop_count, &exec); // free pids?
			error_printer(*shell, DUP2_ERROR, true);
		}
		exec->pipe_flag = 0;
	}
	return (0);
}