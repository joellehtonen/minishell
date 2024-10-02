/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlehtone <jlehtone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 11:55:24 by jlehtone          #+#    #+#             */
/*   Updated: 2024/10/02 11:23:17 by jlehtone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	check_consecutive_separators(t_shell *shell, int index1)
{
	int	index2;
	int	index3;
	
	index2 = index1 + 1;
	index3 = index1 + 2;
	if (isseparator(shell, index1) != false && isseparator(shell, index2) == false
		&& isseparator(shell, index3) == false)
		return (SUCCESS);
	if (isseparator(shell, index1) == PIPE && isseparator(shell, index2) == PIPE
		&& isseparator(shell, index3) == false)
		return (SUCCESS);
	if (isseparator(shell, index1) == REDIR && isseparator(shell, index2) == REDIR
		&& isseparator(shell, index3) == false)
		return (SUCCESS);
	if (isseparator(shell, index1) == PIPE && isseparator(shell, index2) == REDIR
		&& isseparator(shell, index3) == false)
		return (SUCCESS);
	if (isseparator(shell, index1) == PIPE && isseparator(shell, index2) == PIPE
		&& isseparator(shell, index3) == REDIR)
		return (SUCCESS);
	if (isseparator(shell, index1) == PIPE && isseparator(shell, index2) == REDIR
		&& isseparator(shell, index3) == false)
		return (SUCCESS);
	return (FAILURE);
}

static int	check_redir_location(t_shell *shell, int index)
{
	index++;
	while (shell->user_input[index] != '\0')
	{
		if (isseparator(shell, index) == SPACES)
		{ 
			index++;
			if (shell->user_input[index] == '|'
				|| shell->user_input[index] == '<'
				|| shell->user_input[index] == '>')
				return (FAILURE);
		}
		else
			break ;
	}
	if (shell->user_input[index] == '\0')
		return (FAILURE);
	else
		return (SUCCESS);
}

static int	check_pipe_location(t_shell *shell, int old_index)
{
	int new_index;

	new_index = old_index + 1;
	while (shell->user_input[new_index] != '\0')
	{
		if (isseparator(shell, new_index) == SPACES)
		{
			new_index++;
			if (shell->user_input[new_index] == '|')
				return (FAILURE);
		}
		else
			break ;
	}
	if (shell->user_input[new_index] == '\0')
		return (FAILURE);
	new_index = old_index - 1;
	while (new_index >= 0)
	{
		if (isseparator(shell, new_index) == SPACES)
			new_index--;
		else
			return (SUCCESS);
	}
	return (FAILURE);
}

int input_error_check(t_shell *shell)
{
	int	single_quotes;
	int	double_quotes;
	int	index;
	
	index = 0;
	single_quotes = 0;
	double_quotes = 0;
	if (ft_strlen(shell->user_input) == 0)
	{
		// error_printer(shell, EMPTY_INPUT, false);
		//printf("\n");
		return (FAILURE);
	}
	while (shell->user_input[index] != '\0')
	{
		if (shell->user_input[index] == '\'')
		{
			if (shell->user_input[index - 1] != '\\')
				single_quotes++;
		}
		if (shell->user_input[index] == '\"')
		{
			if (shell->user_input[index - 1] != '\\')
				double_quotes++;
		}
		if (shell->user_input[index] == '|')
		{
			if (check_pipe_location(shell, index) == FAILURE)
			{
				error_printer(shell, PIPE_ERROR, false);
				return (FAILURE);
			}
			if (check_consecutive_separators(shell, index) == FAILURE)
			{
				error_printer(shell, SYNTAX_ERROR, false);
				return (FAILURE);
			}
		}
		if (shell->user_input[index] == '<' || shell->user_input[index] == '>')
		{
			if (check_redir_location(shell, index) == FAILURE)
			{
				error_printer(shell, REDIR_ERROR, false);
				return (FAILURE);
			}
			if (check_consecutive_separators(shell, index) == FAILURE)
			{
				error_printer(shell, SYNTAX_ERROR, false);
				return (FAILURE);
			}
		}
		index++;
	}
	if (single_quotes % 2 != 0 || double_quotes % 2 != 0)
	{
		error_printer(shell, QUOTE_ERROR, false);
		return (FAILURE);
	}
	return (SUCCESS);
}

