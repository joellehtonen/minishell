/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_input.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlehtone <jlehtone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 12:55:09 by jlehtone          #+#    #+#             */
/*   Updated: 2024/11/11 12:46:18 by jlehtone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	create_prompt(t_shell *shell);
static void	null_signal(t_shell *shell);
static void	handle_input(t_shell *shell);
static void	clean_empty_nodes(t_shell *shell);

int	read_input(t_shell *shell)
{
	shell->pwd = get_pwd(shell->home, shell);
	while (true)
	{
		create_prompt(shell);
		set_up_signals(shell);
		shell->user_input = readline(shell->prompt); //commented out for a larger tester
		if (g_signal == SIGINT)
		{
			g_signal = 0;
			shell->exit_code = 130;
		}
		// if (isatty(fileno(stdin)))
		// 	shell->user_input = readline(shell->prompt);
		// else
		// {
		// 	char *line;
		// 	line = get_next_line(fileno(stdin));
		// 	shell->user_input = ft_strtrim(line, "\n");
		// 	free(line);
		// }
		if (shell->user_input == NULL)
			null_signal(shell);
		if (input_error_check(shell) == SUCCESS)
			handle_input(shell);
		add_history(shell->user_input);
		free_shell(&shell, false);
	}
	return (shell->exit_code);
}

static void	create_prompt(t_shell *shell)
{
	free_str(&shell->pwd);
	shell->pwd = get_pwd(shell->home, shell);
	shell->envp_str = NULL;
	shell->exec = NULL;
	shell->only_one_builtin = 0;
	if (!shell->uname)
		shell->uname = ft_strdup("user");
	shell->prompt = ft_strjoin_four(shell->uname, ":", shell->pwd, "$ ");
	if (shell->prompt == NULL)
		error_printer(shell, "", MALLOC_FAIL, true);
}

static void	null_signal(t_shell *shell)
{
	printf("exit\n"); // commented out for a larger tester
	shell->exit_code = 130; // commented out for a larger tester
	free_and_exit(shell, 0);
}

static void	handle_input(t_shell *shell)
{
	tokenize_input(shell);
	expander(shell);
	clean_empty_nodes(shell);
	assign_type(&shell->token_pointer);
	assign_level(&shell->token_pointer, &shell->exec, shell);
	//print_node(shell->token_pointer); //for testing
	shell->exit_code = execute(shell);
}

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
