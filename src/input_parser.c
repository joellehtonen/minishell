/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlehtone <jlehtone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 13:19:30 by jlehtone          #+#    #+#             */
/*   Updated: 2024/09/24 11:08:47 by jlehtone         ###   ########.fr       */
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
		if (shell->user_input == '\'') // what if quote is inside a string? can these be condensed into one quote variable?
			single_quotes++;
		if (shell->user_input == '\"')
			double_quotes++;
		// some checks about pipes and redirects?
		index++;
	}
	if (single_quotes % 2 != 0 || double_quotes % 2 != 0)
		free_and_exit(); // error message?
}

int isquote(char character)
{
	if (character == '\'')
		return (1);
	if (character == '\"')
		return (2);
	else
		return (0);
}

void tokenize_input(t_shell *shell)
{
	int		token_index;
	int		start;
	int		end;
	int		quotes_on;
	t_token *new;

	start = 0;
	quotes_on = false;
	input_error_check(shell);
	while (shell->user_input[start] != '\0')
	{
		token_index = 0;
		while (shell->user_input[start] == ' ') // add other white spaces as well, maybe define in the header
			start++;
		end = start;
		while (shell->user_input[end] != ' ' || quotes_on == true)
		{
			if (isquote(shell->user_input[end] != 0) && quotes_on == false)
				quotes_on = true;
			if (isquote(shell->user_input[end] != 0) && quotes_on == true)
				quotes_on = false;
			end++;
		}
		end--;
		new = ft_lstnew_token('0');
		new->line = malloc(sizeof(char) * (end - start + 1));
		if (new->line == NULL)
			free_and_exit(shell); // DO THIS LATER
		while (start != end)
			new->line[token_index++] = shell->user_input[start++];
		new->line[token_index] = '\0';
		ft_lstadd_back_token(&shell->token_pointer, new);
		// new->level++;
	}
}
