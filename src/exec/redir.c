#include "../../inc/minishell.h"

static int	check_for_input(t_shell *shell, int loop_count);

static int	check_for_output(t_shell *shell, int loop_count);

void get_input_and_output(t_shell **shell, int loop_count)
{
	t_exec	*exec;

	exec = (*shell)->exec;

	if (check_for_input(*shell, loop_count) == 0)
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

	if (check_for_output(*shell, loop_count) == 0)
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

static int	check_for_input(t_shell *shell, int loop_count)
{
	t_token	*temp;
	
	temp = find_token_line(shell->token_pointer, loop_count, REDIR, "<<\0");
	if (temp && temp->next)
		return (here_doc(shell->exec, temp));
	else
		temp = find_token_line(shell->token_pointer, loop_count, REDIR, "<\0");
	if (!temp || !temp->next)
		return (1);
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
	return (0);
}

static int	check_for_output(t_shell *shell, int loop_count)
{
	t_token	*temp;
	char	*outfile;
	
	temp = find_token_line(shell->token_pointer, loop_count, REDIR, ">\0");
	if (!temp || !temp->next)
		temp = find_token_line(shell->token_pointer, loop_count, REDIR, ">>\0");;
	if (!temp || !temp->next)
		return (1);
	outfile = temp->next->line;
	if (outfile && outfile[0] == '\0')
	{
		printing(outfile, "", ": No such file or directory\n", 2);
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
	return (0);
}