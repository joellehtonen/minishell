//42 header

#include "../../inc/minishell.h"

// static void	clean_empty_nodes(t_shell *shell)
// {
// 	t_token	*temp;
// 	t_token	*next_node;
// 	t_token *prev_node;

// 	temp = shell->token_pointer;
// 	prev_node = NULL;
// 	while (temp)
// 	{
// 		next_node = temp->next;
// 		if (ft_strlen(temp->line) == 0)
// 		{
// 			delete_one_token(temp);
// 			if (prev_node == NULL)
// 				shell->token_pointer = next_node;
// 			else
// 				prev_node->next = next_node;
// 		}
// 		else
// 			prev_node = temp;
// 		temp = next_node;
// 	}
// }

static void	handle_input(t_shell *shell)
{
	tokenize_input(shell);
	expander(shell);
	//clean_empty_nodes(shell);
	assign_type(&shell->token_pointer);
	assign_level(&shell->token_pointer, &shell->exec, shell);
	//print_node(shell->token_pointer); //for testing
	shell->exit_code = execute(shell);
}

static void	null_signal(t_shell *shell)
{
	printf("exit\n");
	shell->exit_code = 130;
	free_and_exit(shell, 0);
}

static void	create_prompt(t_shell *shell)
{
	free_str(&shell->pwd);
	shell->pwd = get_pwd(shell->home, shell);
	shell->envp_str = NULL;
	shell->exec = NULL;
	shell->only_one_builtin = 0;
	shell->prompt = ft_strjoin_four(shell->uname, ":", shell->pwd, "$ ");
	if (shell->prompt == NULL)
		error_printer(shell, "", MALLOC_FAIL, true);
}

int read_input(t_shell *shell)
{
	shell->pwd = get_pwd(shell->home, shell);
	while (true) 
	{
		create_prompt(shell);
		set_up_signals(shell);
		shell->user_input = readline(shell->prompt);
		if (shell->user_input == NULL)
			null_signal(shell);
		if (input_error_check(shell) == SUCCESS)
			handle_input(shell);
		add_history(shell->user_input);
		free_shell(&shell, false);
	}
	return (shell->exit_code);
}
