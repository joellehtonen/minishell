//42 header

#include "../../inc/minishell.h"

static int	count_str(t_envp *envp_copy);

char	**envp_to_arr(t_envp *envp_copy)
{
	char	**envp_arr;
	t_envp	*temp;
	int		i;

	envp_arr = (char **)malloc((count_str(envp_copy) + 1) * sizeof(char *));
	if (envp_arr == NULL)
		return ((void *) 0);
	i = 0;
	temp = envp_copy;
	while (temp)
	{
		envp_arr[i] = ft_strdup(temp->line);
		if (envp_arr[i] == NULL)
			return (NULL/* free_m(envp_arr, i) */);
		i++;
		temp = temp->next;
	}
	envp_arr[i] = ((void *) 0);
	return (envp_arr);
}

static int	count_str(t_envp *envp_copy)
{
	t_envp	*temp;
	int		count;

	count = 0;
	temp = envp_copy;
	while (temp)
	{
		count++;
		temp = temp->next;
	}
	return (count);
}

int	if_builtin(t_shell *shell, int loop_count)
{
	if ((find_token_line(shell->token_pointer, loop_count, COMM, "cd")) ||\
		(find_token_line(shell->token_pointer, loop_count, COMM, "exit")) ||\
		(find_token_line(shell->token_pointer, loop_count, COMM, "env")) ||\
		(find_token_line(shell->token_pointer, loop_count, COMM, "export")) ||\
		(find_token_line(shell->token_pointer, loop_count, COMM, "unset")) ||\
		(find_token_line(shell->token_pointer, loop_count, COMM, "echo")) ||\
		(find_token_line(shell->token_pointer, loop_count, COMM, "pwd")))
		return(0);
	return(1);
}

void	check_file_access(t_shell *shell, char	*path, int loop_count)
{
	if (access(path, R_OK) == -1 && errno == EACCES)
	{
		//printing(path, "", ": Permission denied\n", 2);
		close_pipes_child(loop_count, &shell->exec); // free pids?
		free_and_exit(shell, 1);
	}
	if (access(path, F_OK) == -1 && errno == ENOENT)
	{
		//printing(path, "", ": No such file or directory\n", 2);
		close_pipes_child(loop_count, &shell->exec); // free pids?
		free_and_exit(shell, 1);
	}
}
