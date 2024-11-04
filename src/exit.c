/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlehtone <jlehtone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 12:40:53 by jlehtone          #+#    #+#             */
/*   Updated: 2024/11/04 12:38:28 by jlehtone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static void	write_error_code(t_shell *shell, char *message)
{
	if ((ft_strncmp(EMPTY_INPUT, message, ft_strlen(message)) == 0)
		|| (ft_strncmp(QUOTE_ERROR, message, ft_strlen(message)) == 0)
		|| (ft_strncmp(MALLOC_FAIL, message, ft_strlen(message)) == 0)
		|| (ft_strncmp(SIGNAL_ERROR, message, ft_strlen(message)) == 0)
		|| (ft_strncmp(TOO_MANY_ARGS, message, ft_strlen(message)) == 0))
		shell->exit_code = 1;
	else if ((ft_strncmp(SYNTAX_ERROR, message, ft_strlen(message)) == 0)
		|| (ft_strncmp(NUMERIC_ERROR, message, ft_strlen(message)) == 0))
		shell->exit_code = 2;
	else if (ft_strncmp(CMD_NOT_FOUND, message, ft_strlen(message)) == 0)
		shell->exit_code = 127;
	else
		shell->exit_code = -1; //not necessary?
}

void	error_printer(t_shell *shell, char *message, int exit)
{
	write(2, message, ft_strlen(message));
	write_error_code(shell, message);
	if (exit == true)
		free_and_exit(shell, 1);
}

void	syntax_error_printer(t_shell *shell, char *message)
{
	write(2, message, ft_strlen(message));
	shell->exit_code = 2;

}

void	free_and_exit(t_shell *shell, int error)
{
	if (shell->only_one_builtin == 0 && shell->user_input)
		free(shell->user_input);
	// what else needs to be freed?
	if (shell->only_one_builtin == 1 && shell->token_pointer && \
		shell->token_pointer->line && ft_strncmp(shell->token_pointer->line, "exit", 4) != 0)
		return ;
	if (shell->exit_code)
		exit(shell->exit_code);
	else
		exit(error);
}

void	free_double_arr(char **arr)
{
	int		i;

	i = 0;
	{
		while (arr[i])
			free(arr[i++]);
		free(arr);
		arr = NULL;
	}
}

int	free_two_str(char *str1, char *str2)
{
	if (str1)
	{
		free(str1);
		str1 = NULL;
	}
	if (str2)
	{
		free(str2);
		str2 = NULL;
	}
	return (1);
}

int	free_str(char *str1)
{
	if (str1)
	{
		free(str1);
		str1 = NULL;
	}
	return (1);
}