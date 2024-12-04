/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklimchu <aklimchu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 12:40:53 by jlehtone          #+#    #+#             */
/*   Updated: 2024/11/25 14:48:08 by aklimchu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	update_error_code(t_shell *shell, char *message);

// prints the error message provided and based on it updates the exit code,
// calls the freeing/exiting program if requested
void	error_printer(t_shell *shell, char *arg, char *message, int exit)
{
	if (shell->only_one_builtin == 0 && (ft_strncmp(message, TOO_MANY_ARGS, ft_strlen(message)) == 0 || \
		ft_strncmp(message, NUMERIC_ERROR, ft_strlen(message)) == 0))
	{
		
	}
	else
	{
		write(2, arg, ft_strlen(arg));
		write(2, message, ft_strlen(message));
	}
	update_error_code(shell, message);
	if (exit == true)
		free_and_exit(shell, 1);
}

// returns an error code based on the error message provided
// otherwise returns default error code 1
static void	update_error_code(t_shell *shell, char *message)
{
	if (ft_strncmp(CMD_NOT_FOUND, message, ft_strlen(message)) == 0
		|| ft_strncmp(NO_FILE_DIR_COMM, message, ft_strlen(message)) == 0)
		shell->exit_code = 127;
	else if (ft_strncmp(IS_DIR_COMM, message, ft_strlen(message)) == 0
		|| ft_strncmp(PERM_DENIED_COMM, message, ft_strlen(message)) == 0)
		shell->exit_code = 126;
	else if ((ft_strncmp(SYNTAX_ERROR, message, ft_strlen(message)) == 0)
		|| (ft_strncmp(NUMERIC_ERROR, message, ft_strlen(message)) == 0)
		|| (ft_strncmp(FILE_ARG, message, ft_strlen(message)) == 0))
		shell->exit_code = 2;
	else
		shell->exit_code = 1;
}

// The function closes remaining file descriptors, 
// frees allocated memory and exits the program
void	free_and_exit(t_shell *shell, int error)
{
	if (shell->only_one_builtin == 1 && shell->token_pointer
		&& shell->token_pointer->line \
		&& ft_strncmp(shell->token_pointer->line, "exit", 4) != 0)
	{
		return ;
	}
	if (shell->only_one_builtin == 1 && shell->token_pointer
		&& shell->token_pointer->line \
		&& ft_strncmp(shell->token_pointer->line, "exit", 4) == 0)
	{
		dup2(shell->exec->orig_in, STDIN_FILENO);
		dup2(shell->exec->orig_out, STDOUT_FILENO);
		close(shell->exec->orig_in);
		close(shell->exec->orig_out);
	}
	free_shell(&shell, true);
	if (shell->exit_code)
		exit(shell->exit_code);
	else
		exit(error);
}
