/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   param_and_path.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklimchu <aklimchu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 10:35:09 by aklimchu          #+#    #+#             */
/*   Updated: 2024/11/13 11:09:17 by aklimchu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	check_empty(t_token *token, int loop_count, t_shell *shell);
static char	*get_exec_path(t_envp *envp_copy, char *command, t_shell *shell);

// The function checks the arguments provided by user for current child process
void	check_param(t_shell *shell, int loop_count)
{
	check_empty(shell->token_pointer, loop_count, shell);
	shell->exec->param = param_to_arr(shell->token_pointer, loop_count);
	if (shell->exec->param == NULL)
		error_printer(shell, "", MALLOC_FAIL, true);
}

// The function checks if there is any token containing a command
static void	check_empty(t_token *token, int loop_count, t_shell *shell)
{
	t_token	*temp;

	temp = token;
	while (temp && temp->level != loop_count)
		temp = temp->next;
	while (temp && temp->level == loop_count)
	{
		if (temp->type == COMM)
			break ;
		temp = temp->next;
	}
	if (!temp || temp->level != loop_count)
		return ;
	if (temp->line[0] == '\0' || temp->line[0] == ' ')
		error_printer(shell, temp->line, CMD_NOT_FOUND, true);
}

// The function finds the path to the command to be executed
char	*check_path(t_envp *paths, char **param, t_shell *shell)
{
	char	*exec_path;
	char	*command;

	command = param[0];
	if (ft_strrchr(command, '/'))
	{
		is_directory(command, shell);
		check_command_access(param, shell);
		return (command);
	}
	if (paths == NULL)
		error_printer(shell, command, NO_FILE_DIR_COMM, true);
	exec_path = get_exec_path(paths, command, shell);
	return (exec_path);
}

// The function browses through various paths contained in PATH variable
// in minishell environment and finds the one where provided command is located
static char	*get_exec_path(t_envp *paths, char *command, t_shell *shell)
{
	char	*exec_path;

	exec_path = NULL;
	while (paths)
	{
		exec_path = ft_strjoin_four(paths->line, "/", "", command);
		if (exec_path && access(exec_path, F_OK) == 0)
			break ;
		free(exec_path);
		exec_path = NULL;
		paths = paths->next;
	}
	if (exec_path == NULL)
		error_printer(shell, command, CMD_NOT_FOUND, true);
	return (exec_path);
}
