/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlehtone <jlehtone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 12:52:56 by jlehtone          #+#    #+#             */
/*   Updated: 2024/11/19 10:22:27 by jlehtone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	exit_loop(t_shell *shell, t_token *token, int i);

// The function executes "exit" builtin
// allowing one + or - ahead of the digit
// numbers bigger than 255 get modulo
int	exit_exec(t_shell *shell, t_token *token)
{
	int	i;
	int	exit_code;

	i = 0;
	exit_code = 0;
	if (token->next)
	{
		if (token->next->next && token->next->next->type != PIPE)
			error_printer(shell, "Exit: ", TOO_MANY_ARGS, true);
		if (token->next->line[i] == '\0')
			error_printer(shell, "Exit: ", NUMERIC_ERROR, true);
		exit_loop(shell, token, i);
		exit_code = ft_atoi(token->next->line);
		if (exit_code > 255)
			exit_code = exit_code % 256;
		shell->exit_code = exit_code;
	}
	free_and_exit(shell, exit_code);
	return (0);
}

static void	exit_loop(t_shell *shell, t_token *token, int i)
{
	int	sign;

	sign = false;
	while (token->next->line[i] != '\0')
	{
		if (token->next->line[i] == '+' || token->next->line[i] == '-')
		{
			if (sign == true)
				error_printer(shell, "Exit: ", NUMERIC_ERROR, true);
			if (sign == false)
			{
				sign = true;
				i++;
			}
		}
		if (ft_isdigit(token->next->line[i]) == false)
			error_printer(shell, "Exit: ", NUMERIC_ERROR, true);
		i++;
	}
}
