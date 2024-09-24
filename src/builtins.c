/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklimchu <aklimchu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 14:46:57 by aklimchu          #+#    #+#             */
/*   Updated: 2024/09/23 15:50:44 by aklimchu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	builtins(t_shell *shell)
{
	if (ft_strncmp(shell->user_input, "cd ", 3) == 0 ||\
		(ft_strncmp(shell->user_input, "cd", 2) == 0 && ft_strlen(shell->user_input) == 2))
		cd_exec(shell);	// int return?
/* 	if (ft_strncmp(shell->user_input, "exit ", 5) == 0)
		exit_exec(shell);
	if (ft_strncmp(shell->user_input, "env ", 4) == 0)
		env_exec(shell);
	if (ft_strncmp(shell->user_input, "export ", 7) == 0)
		export_exec(shell);
	if (ft_strncmp(shell->user_input, "unset ", 6) == 0)
		unset_exec(shell);
	if (ft_strncmp(shell->user_input, "echo ", 5) == 0)
		echo_exec(shell);
	if (ft_strncmp(shell->user_input, "pwd ", 4) == 0)
		pwd_exec(shell); */
	return (0);
}
