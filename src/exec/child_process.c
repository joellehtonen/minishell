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
		free_and_exit(*shell, false);
	}
	
	get_input_and_output(shell, loop_count);

	close_pipes_child(loop_count, &exec);

	if (if_builtin(*shell, loop_count) == 0)
	{
		exit_code = exec_builtins(*shell, loop_count);
		free_and_exit(*shell, exit_code);
	}

	param = check_param(*shell, loop_count);
	if (param == NULL)
		error_printer(*shell, MALLOC_FAIL, true);

	path = check_path((*shell)->path, param, *exec);
	if (path == NULL)
	{
		free_all(param, NULL, NULL, &exec->pid);
		error_printer(*shell, MALLOC_FAIL, true);
	}
	if ((*shell)->envp_str)
		free_double_arr((*shell)->envp_str); // do we need?
	(*shell)->envp_str = envp_to_arr((*shell)->envp_copy);
	if ((*shell)->envp_str == NULL)
	{
		free_all(param, (*shell)->envp_str, NULL, NULL);
		error_printer(*shell, MALLOC_FAIL, true);
	}
	if (execve(path, param, (*shell)->envp_str) == -1)
	{
		printing(param[0], "", ": Permission denied\n", 2);
		free_all(param, (*shell)->envp_str, NULL, NULL); // free path?
		free_and_exit(*shell, 126);
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

