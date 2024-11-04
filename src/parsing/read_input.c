//42 header

#include "../../inc/minishell.h"

int read_input(t_shell *shell)
{
	shell->pwd = get_pwd(shell->home, shell);
	while (true) 
	{
		free_str(&shell->pwd);
		shell->pwd = get_pwd(shell->home, shell);
		shell->envp_str = NULL;
		shell->prompt = ft_strjoin_four(shell->uname, ":", shell->pwd, "$ ");
		if (shell->prompt == NULL)
		{
			error_printer(shell, MALLOC_FAIL, true);
			return (shell->exit_code);
		}
		set_up_signals(shell);
		shell->user_input = readline(shell->prompt);
		if (shell->user_input == NULL)
		{
			printf("exit\n");
			return (shell->exit_code);
		}
		if (input_error_check(shell) == SUCCESS)
		{
			tokenize_input(shell);
			expander(shell);
			assign_type(&shell->token_pointer);
			assign_level(&shell->token_pointer, &shell->exec, shell);
			//print_node(shell->token_pointer); //for testing
			shell->exit_code = execute(shell);
		}
		add_history(shell->user_input);
		free_shell(&shell, false);
	}
	return (shell->exit_code);
}
