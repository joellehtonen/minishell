//42 header

#include "../../inc/minishell.h"

static int	check_no_command(t_token *token, int loop_count);

void	child_process(t_shell **shell, int loop_count)
{
	t_exec	*exec;
	char	**param;
	char	*path;
	int		exit_code;

	exec = (*shell)->exec;

	if (check_no_command((*shell)->token_pointer, loop_count) == 1)
	{
		close_pipes_child(loop_count, &exec);
		exit(0);
	}
	
	get_input_and_output(shell, loop_count);

	close_pipes_child(loop_count, &exec);

	if (if_builtin(*shell, loop_count) == 0)
	{
		exit_code = exec_builtins(*shell, loop_count);
		exit(exit_code);
	}

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

