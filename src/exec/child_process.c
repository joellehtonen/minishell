//42 header

#include "../inc/minishell.h"

static int	check_no_command(t_token *token, int loop_count);

static int	check_for_input(t_shell *shell, int loop_count, int flag_pipe);

static int	check_for_output(t_shell *shell, int loop_count, int flag_pipe);

void	child_process(t_shell **shell, int loop_count, int flag_pipe)
{
	t_exec	*exec;
	char	**param;
	char	*path;
	
	exec = (*shell)->exec;
	close_free(flag_pipe, exec->pipe[0], -1, &exec->null);
	if (check_no_command((*shell)->token_pointer, loop_count) == 1)
	{
		if (flag_pipe == 1)
			close(exec->pipe[1]);
		exit (0);
	}
	if (check_for_input(*shell, loop_count, flag_pipe) == 0)
	{
		if (dup2(exec->in, 0) == -1)
		{
			close_free(flag_pipe, exec->pipe[1], -1, &exec->null);
			close(exec->in);
			perror("dup() error");
			exit(1);
		}
		close(exec->in);
	}
	if (check_for_output(*shell, loop_count, flag_pipe) == 0)
	{
		if (dup2(exec->out, 1) == -1)
		{
			close_free(flag_pipe, exec->pipe[1], -1, &exec->null);
			close(exec->out);
			perror("dup() error");
			exit(1);
		}
		close(exec->out);
	}
	else if (flag_pipe == 1)
	{
		if (dup2(exec->pipe[1], 1) == -1)
		{
			close_free(flag_pipe, exec->pipe[1], -1, &exec->pid);
			perror("dup() error");
			exit(1);
		}
		close(exec->pipe[1]);
	}

	close_free(flag_pipe, exec->pipe[1], -1, &exec->null); //do we need?

	param = check_param(*shell, loop_count);
	if (param == NULL)
	{
		/* free_pid(&exec->pid); */
		exit(1);
	}

	path = check_path((*shell)->path, param, *exec);
	if (path == NULL)
	{
		/* free_all(param, NULL, NULL, &fd.pid); */
		exit(1);
	}
	if ((*shell)->envp_str)
		free_double_arr((*shell)->envp_str);
	(*shell)->envp_str = envp_to_arr((*shell)->envp_copy);
	if ((*shell)->envp_str == NULL)
	{
		perror("malloc() failed");
		//free_all
		exit(1);
	}
	if (execve(path, param, (*shell)->envp_str) == -1)
	{
		printing(param[0], "", ": Permission denied\n", 2);
		/* free_all(param, NULL, NULL, &fd.pid); */
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

static int	check_for_input(t_shell *shell, int loop_count, int flag_pipe)
{
	t_token	*temp;
	
	temp = shell->token_pointer;
	while (temp && temp->level != loop_count)
		temp = temp->next;
	while (temp && temp->level == loop_count)
	{
		if (temp->type == REDIR && ft_strncmp(temp->line, "<\0", 2) == 0)
			break ;
		temp = temp->next;
	}
	if (!temp || temp->level != loop_count)
		return (1);
	if (access(temp->next->line, R_OK) == -1 && errno == EACCES)
		printing(temp->next->line, "", ": Permission denied\n", 2);
	if (access(temp->next->line, F_OK) == -1 && errno == ENOENT)
		printing(temp->next->line, "", ": No such file or directory\n", 2);
	shell->exec->in = open(temp->next->line, O_RDONLY);
	if (shell->exec->in == -1)
	{
		close_free(flag_pipe, shell->exec->pipe[1], -1, &shell->exec->pid);
		exit(1);
	}
	return (0);
}

static int	check_for_output(t_shell *shell, int loop_count, int flag_pipe)
{
	t_token	*temp;
	char	*outfile;
	
	temp = shell->token_pointer;
	while (temp && temp->level != loop_count)
		temp = temp->next;
	while (temp && temp->level == loop_count)
	{
		if (temp->type == REDIR && ft_strncmp(temp->line, ">\0", 2) == 0)
			break ;
		temp = temp->next;
	}
	if (!temp || temp->level != loop_count)
		return (1);
	outfile = temp->next->line;
	if (outfile && outfile[0] == '\0')
	{
		printing(outfile, "", ": No such file or directory\n", 2);
		close_free(flag_pipe, shell->exec->pipe[0], -1, &shell->exec->pid);
		exit(1);
	}
	else
	{
		shell->exec->out = open(outfile, O_WRONLY | O_TRUNC | O_CREAT, 0777);
		if (shell->exec->out == -1)
		{
			is_directory(outfile, *shell->exec, shell->exec->pipe[0], NULL);
			if (access(outfile, W_OK) == -1 && errno == EACCES)
				printing(outfile, "", ": Permission denied\n", 2);
			close_free(flag_pipe, shell->exec->pipe[0], -1, &shell->exec->pid);
			exit(1);
		}
	}
	return (0);
}