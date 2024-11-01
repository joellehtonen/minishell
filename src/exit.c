/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlehtone <jlehtone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 12:40:53 by jlehtone          #+#    #+#             */
/*   Updated: 2024/11/01 14:48:39 by jlehtone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	 error_printer(t_shell *shell, char *message, int exit)
{
	// printf("Error. %s\n", message);
	write(2, message, ft_strlen(message));
	if (exit == true)
		free_and_exit(shell, 1);
}

void	free_and_exit(t_shell *shell, int error)
{
	if (shell->user_input)
		free(shell->user_input);
	// what else needs to be freed?
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