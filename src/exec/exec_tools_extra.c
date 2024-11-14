/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_tools_extra.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklimchu <aklimchu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 12:56:43 by aklimchu          #+#    #+#             */
/*   Updated: 2024/11/13 10:42:21 by aklimchu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	count_nodes(t_envp *envp_copy);

// The function convert the linked list with minishell environment
// variables to double array
char	**envp_to_arr(t_envp *envp_copy)
{
	char	**envp_arr;
	t_envp	*temp;
	int		i;

	envp_arr = (char **)malloc((count_nodes(envp_copy) + 1) * sizeof(char *));
	if (envp_arr == NULL)
		return ((void *) 0);
	i = 0;
	temp = envp_copy;
	while (temp)
	{
		envp_arr[i] = ft_strdup(temp->line);
		if (envp_arr[i] == NULL)
			return (free_double_arr_part(&envp_arr, i - 1));
		i++;
		temp = temp->next;
	}
	envp_arr[i] = ((void *) 0);
	return (envp_arr);
}

// The function counts the number of nodes in linked list
// containing minishell environment
static int	count_nodes(t_envp *envp_copy)
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

// The function is checking if the command provided by user is a builtin
int	if_builtin(t_shell *shell, int loop_count)
{
	if ((find_token_line(shell->token_pointer, loop_count, COMM, "cd")) || \
		(find_token_line(shell->token_pointer, loop_count, COMM, "exit")) || \
		(find_token_line(shell->token_pointer, loop_count, COMM, "env")) || \
		(find_token_line(shell->token_pointer, loop_count, COMM, "export")) || \
		(find_token_line(shell->token_pointer, loop_count, COMM, "unset")) || \
		(find_token_line(shell->token_pointer, loop_count, COMM, "echo")) || \
		(find_token_line(shell->token_pointer, loop_count, COMM, "pwd")))
		return (0);
	return (1);
}

// The function checks the file access
void	check_file_access(t_shell *shell, char	*path, int loop_count)
{
	if (access(path, R_OK) == -1 && errno == EACCES)
	{
		close_pipes_child(loop_count, &shell->exec);
		free_and_exit(shell, 1);
	}
	if (access(path, F_OK) == -1 && errno == ENOENT)
	{
		close_pipes_child(loop_count, &shell->exec);
		free_and_exit(shell, 1);
	}
}
