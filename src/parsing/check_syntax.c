/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlehtone <jlehtone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 11:55:24 by jlehtone          #+#    #+#             */
/*   Updated: 2024/09/27 14:15:53 by jlehtone         ###   ########.fr       */
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
		return (failure);
	while (shell->user_input[back] != '\0')
	{
		if (isseparator(shell->user_input[back]) == SPACES)
			back++;
		else
			break ;
	}
	if (shell->user_input[back] == '\0')
		return (failure);
	while (front >= 0)
	{
		if (isseparator(shell->user_input[front]) == SPACES)
			front--;
		else
			return (success);
	}
	return (failure);
}

void input_error_check(t_shell *shell)
{
	int	single_quotes;
	int	double_quotes;
	int	index;
	
	index = 0;
	single_quotes = 0;
	double_quotes = 0;
	if (ft_strlen(shell->user_input) == 0)
		free_and_exit(shell); // do we need an error message?
	while (shell->user_input[index] != '\0')
	{
		if (shell->user_input[index] == '\'') // what if quote is inside a string? can these be condensed into one quote variable?
			single_quotes++;
		if (shell->user_input[index] == '\"')
			double_quotes++;
		if (shell->user_input[index] == '|')
		{
			if (check_pipe_location(shell, index) == failure)
			{
				perror("ERROR. Pipe cannot be first or last\n");
				exit(1);
			}
		}
		// check if pipe is not at the end of sentence (no only whitespaces and null)
		// check if pipe is not the first thing on the sentence (not just whitespaces before it)
		// no pipe followed by only white space and then another pipe
		// check if redir is not at the end
		// check if redir is not at the start
		// no redirs into folders (check for /)
		// check if double redir >> is not at the end 
		// (double redir needs to be valid input also)
		// redir cannot immediately follow pipe
		// no redirs before pipe
		// no two redirs into same way in the same command?
		// check permissions before redirecting
		index++;
	}
	if (single_quotes % 2 != 0 || double_quotes % 2 != 0)
	{
		perror("ERROR. Odd number of quotes\n");
		exit(1);
		//free_and_exit(shell); // with an error message?
	}
}

