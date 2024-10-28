#include "../../inc/minishell.h"

static int	check_for_input(t_shell *shell, t_token *token, int loop_count, int input_flag);

static int	check_for_output(t_shell *shell, t_token *token, int loop_count, int output_flag);

void get_input_and_output(t_shell **shell, int loop_count)
{
	t_exec	*exec;

	exec = (*shell)->exec;

	if (check_for_input(*shell, (*shell)->token_pointer, loop_count, 0) == 0)
	{
		if (dup2(exec->in, 0) == -1)
		{
			close_pipes_child(loop_count, &exec); // free pids?
			close(exec->in);
			perror("dup() error");
			exit(1);
		}
		close(exec->in);
	}
	else if (loop_count > 0 && dup2(exec->pipe[loop_count - 1][0], 0) == -1)
	{
		close_pipes_child(loop_count, &exec); // free pids?
		perror("dup() error 111");
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

static int	check_for_input(t_shell *shell, t_token *token, int loop_count, int input_flag)
{
	t_token	*temp;

	temp = find_token(token, loop_count, REDIR_INPUT);
	if (input_flag == 0 && (!temp || !temp->next || (temp->next->type != INPUT &&\
		temp->next->type != DELIM)))
		return (1);
	if (input_flag == 1 && (!temp || !temp->next || (temp->next->type != INPUT &&\
		temp->next->type != DELIM)))
		return (0);
	if (input_flag == 1)
		close(shell->exec->in);

	if (ft_strncmp(temp->line, "<<\0", 3) == 0)
	{
		here_doc(shell->exec, temp);
		return (check_for_input(shell, temp->next, loop_count, 1));
	}
	if (access(temp->next->line, R_OK) == -1 && errno == EACCES)
		printing(temp->next->line, "", ": Permission denied\n", 2);
	if (access(temp->next->line, F_OK) == -1 && errno == ENOENT)
		printing(temp->next->line, "", ": No such file or directory\n", 2);
	shell->exec->in = open(temp->next->line, O_RDONLY);
	if (shell->exec->in == -1)
	{
		close_pipes_child(loop_count, &shell->exec); // free pids?
		exit(1);
	}
	check_for_input(shell, temp->next, loop_count, 1);
	return (0);
}

static int	check_for_output(t_shell *shell, t_token *token, int loop_count, int output_flag)
{
	t_token	*temp;
	char	*outfile;
	
	temp = find_token(token, loop_count, REDIR_OUTPUT);
	if (output_flag == 0 && (!temp || !temp->next || temp->next->type != OUTPUT))
		return (1);
	if (output_flag == 1 && (!temp || !temp->next || temp->next->type != OUTPUT))
		return (0);
	if (output_flag == 1)
		close(shell->exec->out);
	outfile = temp->next->line;
	if (outfile && outfile[0] == '\0')
	{
		printing(outfile, "", ": No such file or directory\n", 2); // what if $HOME?
		close_pipes_child(loop_count, &shell->exec); // free pids?
		exit(1);
	}
	else
	{
		if (ft_strncmp(temp->line, ">\0", 2) == 0)
			shell->exec->out = open(outfile, O_WRONLY | O_TRUNC | O_CREAT, 0777);
		else
			shell->exec->out = open(outfile, O_WRONLY | O_APPEND | O_CREAT, 0777);
		if (shell->exec->out == -1)
		{
			is_directory(outfile, *shell->exec, shell->exec->pipe[loop_count][0], NULL);
			if (access(outfile, W_OK) == -1 && errno == EACCES)
				printing(outfile, "", ": Permission denied\n", 2);
			close_pipes_child(loop_count, &shell->exec); // free pids?
			exit(1);
		}
	}
	check_for_output(shell, temp->next, loop_count, 1);
	return (0);
}