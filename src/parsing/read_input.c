/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_input.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlehtone <jlehtone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 12:55:09 by jlehtone          #+#    #+#             */
/*   Updated: 2024/11/20 10:40:07 by jlehtone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	create_prompt(t_shell *shell);
void		null_signal(t_shell *shell, char *arg);
static void	handle_input(t_shell *shell);
static void	clean_empty_nodes(t_shell *shell);

// reads user input using readline, checks it for syntax errors,
// and passes it forward, adding it to history
// returns the final exit code for minishell
int	read_input(t_shell *shell)
{
	shell->pwd = get_pwd(shell->home, shell);
	while (true)
	{
		create_prompt(shell);
		set_up_signals(shell);
		shell->user_input = readline(shell->prompt);
		free_str(&shell->prompt);
		if (g_signal != 0)
			handle_readline_signal(shell);
		if (shell->user_input == NULL)
			null_signal(shell, "");
		if (input_error_check(shell) == SUCCESS)
			handle_input(shell);
		add_history(shell->user_input);
		free_shell(&shell, false);
	}
	return (shell->exit_code);
}

// creates the minishell prompt
static void	create_prompt(t_shell *shell)
{
	if (fill_values_before_prompt(&shell) == 1)
		error_printer(shell, "", MALLOC_FAIL, true);
	shell->prompt = ft_strjoin_four(shell->uname, ":", shell->pwd, "$ ");
	free_str(&shell->uname);
	if (shell->prompt == NULL)
		error_printer(shell, "", MALLOC_FAIL, true);
}

// calls the functions to tokenize, expand and clean the user input
// assigns types and levels for each token
// and passes them to execute function
static void	handle_input(t_shell *shell)
{
	tokenize_input(shell);
	expander(shell);
	clean_empty_nodes(shell);
	assign_type(&shell->token_pointer);
	assign_level(&shell->token_pointer, &shell->exec, shell);
	shell->exit_code = execute(shell);
}

// cleans empty nodes if there are any after expanding
static void	clean_empty_nodes(t_shell *shell)
{
	t_token	*temp;
	t_token	*next_node;
	t_token	*prev_node;

	temp = shell->token_pointer;
	prev_node = NULL;
	while (temp)
	{
		next_node = temp->next;
		if (ft_strlen(temp->line) == 0 && temp->expanded == true)
		{
			delete_one_token(temp);
			if (prev_node == NULL)
				shell->token_pointer = next_node;
			else
				prev_node->next = next_node;
		}
		else
			prev_node = temp;
		temp = next_node;
	}
}
