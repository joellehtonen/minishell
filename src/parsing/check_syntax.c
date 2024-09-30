/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklimchu <aklimchu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 11:55:24 by jlehtone          #+#    #+#             */
/*   Updated: 2024/09/30 15:21:34 by aklimchu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static int check_pipe_location(t_shell *shell, int index)
{
	int	front;
	int	back;

	back = index + 1;
	front = index - 1;
	if (front < 0)
		return (FAILURE);
	while (shell->user_input[back] != '\0')
	{
		if (isseparator(shell->user_input[back]) == /* SPACES */1)
		{
			back++;
			if (shell->user_input[back] == '|')
				return (FAILURE);
		}
		else
			break ;
	}
	if (shell->user_input[back] == '\0')
		return (FAILURE);
	while (front >= 0)
	{
		if (isseparator(shell->user_input[front]) == /* SPACES */1)
			front--;
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
		printf("ERROR. Empty input\n");
		return (FAILURE);
	}
	while (shell->user_input[index] != '\0')
	{
		if (shell->user_input[index] == '\'') // what if quote is inside a string?
			single_quotes++;
		if (shell->user_input[index] == '\"')
			double_quotes++;
		if (shell->user_input[index] == '|')
		{
			if (check_pipe_location(shell, index) == FAILURE)
			{
				printf("ERROR. Pipe cannot be first or last, or follow another pipe\n");
				return (FAILURE);
			}
		}
		// no pipe followed by only white space and then another pipe
		// check if redir is not at the end
		// check if redir is not at the start
		// check if double redir >> is not at the end or start
		// redir cannot immediately follow pipe
		// no redirs before pipe
		index++;
	}
	if (single_quotes % 2 != 0 || double_quotes % 2 != 0)
	{
		printf("ERROR. Odd number of quotes\n");
		return (FAILURE);
	}
	return (SUCCESS);
}

