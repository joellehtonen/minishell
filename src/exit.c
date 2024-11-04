/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklimchu <aklimchu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 12:40:53 by jlehtone          #+#    #+#             */
/*   Updated: 2024/10/31 11:40:24 by aklimchu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	 error_printer(t_shell *shell, char *message, int exit)
{
	// printf("Error. %s\n", message);
	write(2, message, ft_strlen(message));
	write(2, "\n", 1);
	if (exit == true)
		free_and_exit(shell, 1);
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