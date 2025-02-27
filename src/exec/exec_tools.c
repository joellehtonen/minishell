/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_tools.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlehtone <jlehtone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 09:39:08 by aklimchu          #+#    #+#             */
/*   Updated: 2024/11/14 14:19:01 by jlehtone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	count_tokens(t_token *token, int loop_count);
static char	**free_param_arr(char **param_arr, int i);

// The function checks if provided path leads to a directory
int	is_directory(char *path, t_shell *shell)
{
	int		test_fd;
	char	buffer;
	ssize_t	result;

	test_fd = open(path, O_RDONLY);
	if (test_fd < 0)
		return (-1);
	result = read(test_fd, &buffer, 1);
	if (result < 0 && errno == EISDIR)
	{
		close(test_fd);
		error_printer(shell, path, IS_DIR_COMM, true);
	}
	close(test_fd);
	return (0);
}

// The function looks for the nodes containing command and arguments
// and saves the data into a double array
char	**param_to_arr(t_token *token, int loop_count)
{
	char	**param_arr;
	t_token	*temp;
	int		i;

	param_arr = (char **)malloc((count_tokens(token, loop_count) + 1) \
		* sizeof(char *));
	if (param_arr == NULL)
		return ((void *) 0);
	temp = find_token(token, loop_count, COMM);
	i = 0;
	while (temp && temp->level == loop_count)
	{
		if (temp->type == COMM || temp->type == ARG)
		{
			param_arr[i] = ft_strdup(temp->line);
			if (param_arr[i] == NULL)
				return (free_param_arr(param_arr, i - 1));
			i++;
		}
		temp = temp->next;
	}
	param_arr[i] = ((void *) 0);
	return (param_arr);
}

// The function frees double array containing a known amount of strings
static char	**free_param_arr(char **param_arr, int i)
{
	while (i >= 0)
		free(param_arr[i--]);
	free(param_arr);
	return (NULL);
}

// The function counts the number of tokens in the linked list 
// containing tokenized user input (limited by loop / child process number)
static int	count_tokens(t_token *token, int loop_count)
{
	t_token	*temp;
	int		count;

	temp = find_token(token, loop_count, COMM);
	count = 0;
	while (temp && temp->level == loop_count)
	{
		if (temp->type == COMM || temp->type == ARG)
			count++;
		temp = temp->next;
	}
	return (count);
}

// The function is checking access status to the path containing command
void	check_command_access(char **param, t_shell *shell)
{
	char	*command;

	command = param[0];
	if (access(command, F_OK) == -1 && errno == ENOENT)
		error_printer(shell, command, NO_FILE_DIR_COMM, true);
}
