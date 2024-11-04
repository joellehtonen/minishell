/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlehtone <jlehtone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 11:55:24 by jlehtone          #+#    #+#             */
/*   Updated: 2024/11/04 13:52:58 by jlehtone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	validate_io(t_shell *shell, int index)
{
	int	fail;

	fail = false;
	if (shell->single_quote == true || shell->double_quote == true)
		return (SUCCESS);
	if (is_io(shell->user_input[index]) == false)
		return (SUCCESS);
	if (shell->user_input[index] == '|')
	{
		if (check_pipe_location(shell, index) == FAILURE)
			fail = true;
	}
	if (shell->user_input[index] == '<' || shell->user_input[index] == '>')
	{
		if (check_redir_location(shell, index) == FAILURE)
			fail = true;
	}
	if (check_consecutive_io(shell, index) == FAILURE)
		fail = true;
	if (fail == true)
	{
		error_printer(shell, SYNTAX_ERROR, false);
		return (FAILURE);
	}
	return (SUCCESS);
}

static void	count_quotes(t_shell *shell, int index, int *s_quote, int *d_quote)
{
	if (shell->user_input[index] == '\'')
	{
		if (index == 0 || shell->user_input[index - 1] != '\\')
		{
			if (shell->double_quote == false)
			{
				*s_quote = *s_quote + 1;
				shell->single_quote = !shell->single_quote;
			}
		}
	}
	if (shell->user_input[index] == '\"')
	{
		if (index == 0 || shell->user_input[index - 1] != '\\')
		{
			if (shell->single_quote == false)
			{
				*d_quote = *d_quote + 1;
				shell->double_quote = !shell->double_quote;
			}
		}
	}
}

int	input_error_check(t_shell *shell)
{
	int	single_quotes;
	int	double_quotes;
	int	index;

	index = 0;
	single_quotes = 0;
	double_quotes = 0;
	reset_quotes(shell);
	if (ft_strlen(shell->user_input) == 0)
		return (FAILURE);
	while (shell->user_input[index] != '\0')
	{
		count_quotes(shell, index, &single_quotes, &double_quotes);
		if (validate_io(shell, index) == FAILURE)
			return (FAILURE);
		index++;
	}
	if (single_quotes % 2 != 0 || double_quotes % 2 != 0)
	{
		error_printer(shell, QUOTE_ERROR, false);
		return (FAILURE);
	}
	return (SUCCESS);
}
