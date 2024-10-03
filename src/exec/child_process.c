//42 header

#include "../../inc/minishell.h"

static int	check_no_command(t_token *token, int loop_count);

static int	check_for_input(t_shell *shell, int loop_count);

static int	check_for_output(t_shell *shell, int loop_count);

void	child_process(t_shell **shell, int loop_count, int flag_pipe)
{
	t_exec	*exec;
	char	**param;
	char	*path;
	

	exec = (*shell)->exec;

	if (check_no_command((*shell)->token_pointer, loop_count) == 1)
	{
		close_pipes_child(loop_count, &exec);
		exit (0);
	}
	

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
	else if (flag_pipe == 1)
	{
		if (dup2(exec->pipe[loop_count][1], 1) == -1)
		{
			close_pipes_child(loop_count, &exec); // free pids?
			perror("dup() error");
			exit(1);
		}
		flag_pipe = 0;
	}
	
	close_pipes_child(loop_count, &exec);

	//check if builtins

	param = check_param(*shell, loop_count);
	if (param == NULL)
	{
		//free_exec(&exec);
		exit(1);
	}

	path = check_path((*shell)->path, param, *exec);
	if (path == NULL)
	{
		free_all(param, NULL, NULL, &exec->pid);
		exit(1);
	}
	if ((*shell)->envp_str)
		free_double_arr((*shell)->envp_str); // do we need?
	(*shell)->envp_str = envp_to_arr((*shell)->envp_copy);
	if ((*shell)->envp_str == NULL)
	{
		perror("malloc() failed");
		free_all(param, (*shell)->envp_str, NULL, &exec->pid);
		exit(1);
	}
	if (execve(path, param, (*shell)->envp_str) == -1)
	{
		printing(param[0], "", ": Permission denied\n", 2);
		free_all(param, (*shell)->envp_str, NULL, &exec->pid); // free path?
		exit(126);
	}
}

static int	check_no_command(t_token *token, int loop_count)
{
	t_token	*temp;
	
	temp = token;
	while (temp && temp->level != loop_count)
		temp = temp->next;
	while (temp && temp->level == loop_count)
	{
		if (temp->type == COMM)
			return (0);
		temp = temp->next;
	}
	return (1);
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