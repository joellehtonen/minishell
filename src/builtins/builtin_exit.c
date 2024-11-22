/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlehtone <jlehtone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 12:52:56 by jlehtone          #+#    #+#             */
/*   Updated: 2024/11/21 15:59:26 by jlehtone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void			exit_loop(t_shell *shell, t_token *token, int i);
static int			find_exit_code(char *str, t_shell *shell);
static long			ft_atol(const char *str);
static const char	*skip_white_spaces(const char *str);

// The function executes "exit" builtin
// allowing one + or - ahead of the digit
// numbers bigger than 255 get modulo
int	exit_exec(t_shell *shell, t_token *token)
{
	int		i;

	i = 0;
	if (token->next)
	{
		exit_loop(shell, token, i);
		if (token->next->next && token->next->next->type != PIPE)
		{
			printf("exit\n");
			error_printer(shell, "Exit: ", TOO_MANY_ARGS, false);
			return (1);
		}
		if (token->next->line[i] == '\0')
		{
			printf("exit\n");
			error_printer(shell, "Exit: ", NUMERIC_ERROR, true);
		}
		shell->exit_code = find_exit_code(token->next->line, shell);
	}
	printf("exit\n");
	free_and_exit(shell, 0);
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
			{
				printf("exit\n");
				error_printer(shell, "Exit: ", NUMERIC_ERROR, true);
			}
			if (sign == false)
			{
				sign = true;
				i++;
			}
		}
		if (ft_isdigit(token->next->line[i]) == false)
		{
			printf("exit\n");
			error_printer(shell, "Exit: ", NUMERIC_ERROR, true);
		}
		i++;
	}
}

static int	find_exit_code(char *input, t_shell *shell)
{
	long	exit_code_long;
	int		exit_code;

	exit_code = 0;
	exit_code_long = ft_atol(input);
	if ((exit_code_long < 0 && input[0] != '-')
		|| (exit_code_long == 0 && input[0] == '-'))
	{
		printf("exit\n");
		error_printer(shell, "Exit: ", NUMERIC_ERROR, true);
	}
	else if (exit_code_long > 255 || exit_code_long < 0)
		exit_code = exit_code_long % 256;
	else
		exit_code = exit_code_long;
	return (exit_code);
}

static long	ft_atol(const char *str)
{
	long int	res;
	int			neg;

	res = 0;
	neg = 1;
	str = skip_white_spaces(str);
	if (*str == 45)
	{
		neg = neg * (-1);
		str++;
	}
	while (*str > 47 && *str < 58)
	{
		res = res * 10 + (*str - '0');
		if (res < 0 && neg == 1)
			return (-1);
		if (res < 0 && neg == -1)
			return (0);
		str++;
	}
	return (res * neg);
}

static const char	*skip_white_spaces(const char *str)
{
	while ((*str == 32) || (*str > 8 && *str < 14))
		str++;
	if (*str == 43 && *(str + 1) == 45)
		return (str);
	else if (*str == 43)
		str++;
	return (str);
}
