/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlehtone <jlehtone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 13:19:30 by jlehtone          #+#    #+#             */
/*   Updated: 2024/09/23 11:37:00 by jlehtone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void input_error_check(t_shell *shell)
{
	int	single_quotes;
	int	double_quotes;
	int	index;
	
	index = 0;
	single_quotes = 0;
	double_quotes = 0;
	if (ft_strlen(shell->user_input == 0))
		free_and_exit(); // do we need an error message?
	while (shell->user_input)
	{
		if (shell->user_input == '\'') //what if quote is inside a string? can these be condensed into one quote variable?
			single_quotes++;
		if (shell->user_input == '\"')
			double_quotes++;
		//some checks about pipes and redirects?
		index++;
	}
	if (single_quotes % 2 != 0 || double_quotes % 2 != 0)
		free_and_exit(); //error message? bash is waiting for another quote, I believe...
}

void tokenize_input(t_shell *shell)
{
	int	index;

	index = 0;
	input_error_check(shell);
	while (shell->user_input)
	{
		if (shell->user_input[index] == ' ') // add other white spaces as well, maybe define in the header
			index++;
		
		index++;
	}
	// check if there is an even amount of quotes, brackets, etc
}
