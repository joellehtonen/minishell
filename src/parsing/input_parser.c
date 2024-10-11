/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklimchu <aklimchu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 13:19:30 by jlehtone          #+#    #+#             */
/*   Updated: 2024/10/11 15:34:04 by aklimchu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../inc/minishell.h"

static int	create_new_token(t_shell *shell, int end, int start, int token_number)
{
	t_token *new;
	int		token_index;

	token_index = 0;
	new = ft_lstnew_token(NULL);
	new->line = malloc(sizeof(char) * (end - start + 1));
	if (new->line == NULL)
		error_printer(shell, MALLOC_FAIL, true);
	while (start != end)
		new->line[token_index++] = shell->user_input[start++];
	new->line[token_index] = '\0';
	new->token_number = token_number;
	ft_lstadd_back_token(&shell->token_pointer, new);
	return (start);
}

static int handle_argument(t_shell *shell, int end)
{
	int		quotes_on;
	int		IO_met;

	quotes_on = false;
	IO_met = false;
	//while (shell->user_input[end] != '\0' && (shell->user_input[end] != ' ' || quotes_on == true))
	while (shell->user_input[end] != '\0' && (ft_isspace(shell->user_input[end]) == false || quotes_on == true))
	{
		if (isquote(shell->user_input[end]) != false)
			quotes_on = !quotes_on;
		if (isIO(shell->user_input[end]) != false && quotes_on == false)
			{
				if (IO_met == true)
					end++;
				IO_met = !IO_met;
				break ;
			}
		end++;
	}
	return (end);
}

void tokenize_input(t_shell *shell)
{
	int		start;
	int		end;
	int		token_number;

	start = 0;
	token_number = 0;
	while (shell->user_input[start] != '\0')
	{
		while (ft_isspace(shell->user_input[start]) == true)
			start++;
		if (shell->user_input[start] == '\0')
			break ;
		else
			end = start;
		if (isIO(shell->user_input[end]) != false)
		{
			end++;
			if (shell->user_input[end] == shell->user_input[end - 1])
				end++;
		}
		else
			end = handle_argument(shell, end);
		start = create_new_token(shell, end, start, token_number);
		token_number++;
	}
}
