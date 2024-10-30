#include "../../inc/minishell.h"

static int	check_for_here_doc(t_token *token, int loop_count);

void get_input_and_output(t_shell **shell, int loop_count)
{
	t_exec	*exec;
	int		here_doc_index;

	exec = (*shell)->exec;
	here_doc_index = check_for_here_doc((*shell)->token_pointer, loop_count);
	//ft_printf("here_doc index: %d\n", here_doc_index);
	if (here_doc_index >= 0)
	{
		if (dup2(exec->here_doc_pipe[here_doc_index][0], 0) == -1)
		{
			close_pipes_child(loop_count, &exec); // free pids?
			//close([here_doc_index][0]);
			perror("dup() error");
			exit(1);
		}
		//close([here_doc_index][0]);
	}
	else if (check_for_input(*shell, (*shell)->token_pointer, loop_count, 0) == 0)
	{
		if (dup2(exec->in, 0) == -1)
		{
			close_pipes_child(loop_count, &exec); // free pids?
			close(exec->in);
			perror("dup() error");
			exit(1);
		}
		//printf("fd for output file copied\n");
		close(exec->in);
	}
	else if (loop_count > 0 && dup2(exec->pipe[loop_count - 1][0], 0) == -1)
	{
		close_pipes_child(loop_count, &exec); // free pids?
		perror("dup() error");
		exit(1);
	}

	if (check_for_output(*shell, (*shell)->token_pointer, loop_count, 0) == 0)
	{
		if (dup2(exec->out, 1) == -1)
		{
			close_pipes_child(loop_count, &exec); // free pids?
			close(exec->out);
			perror("dup() error");
			exit(1);
		}
		close(exec->out);
	}
	else if (exec->pipe_flag == 1)
	{
		if (dup2(exec->pipe[loop_count][1], 1) == -1)
		{
			close_pipes_child(loop_count, &exec); // free pids?
			perror("dup() error");
			exit(1);
		}
		exec->pipe_flag = 0;
	}
}

static int	check_for_here_doc(t_token *token, int loop_count)
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
			res = temp;
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