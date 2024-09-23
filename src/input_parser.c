/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlehtone <jlehtone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 13:19:30 by jlehtone          #+#    #+#             */
/*   Updated: 2024/09/23 16:52:08 by jlehtone         ###   ########.fr       */
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
		free_and_exit(); //error message?
}

void tokenize_input(t_shell *shell)
{
	int		index;
	int		token_index;
	int		start;
	int		end;
	t_token *new;

	index = 0;
	input_error_check(shell);
	shell->token_pointer = NULL;
	while (shell->user_input[index] != '\0')
	{
		token_index = 0;
		while (shell->user_input[index] == ' ') // add other white spaces as well, maybe define in the header
			index++;
		start = index;
		while (isalpha(shell->user_input[index]) == true)
			index++;
		end = index;
		new = ft_lstnew_token(NULL);
		new->line = malloc(sizeof(char) * (end - start + 1));
		if (new->line == NULL)
			free_and_exit(shell); // DO THIS LATER
		index = start;
		while (isalpha(shell->user_input[index]) == true)
			new->line[token_index++] = shell->user_input[index++];
		new->line[token_index] = '\0';
		ft_lstadd_back_token(&shell->token_pointer, new);
		new->level++;
		// if (isseparator(shell->user_input[index]) == true)
	}
}
