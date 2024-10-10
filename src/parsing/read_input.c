//42 header

#include "../../inc/minishell.h"

int read_input(t_shell *shell)
{
	char	*prompt;

	shell->pwd = get_pwd(shell->home);
	while (true) 
	{
		free(shell->pwd);
		shell->pwd = get_pwd(shell->home);
		shell->envp_str = NULL;
		prompt = ft_strjoin_four(shell->uname, ":", shell->pwd, "$ ");
		set_up_signals(shell);
		shell->user_input = readline(prompt);
		if (shell->user_input == NULL)
		{
			printf("exit\n");
			free_and_exit(shell, 0);
		}
		if (input_error_check(shell) == SUCCESS)
		{
			tokenize_input(shell);
			//expand
			assign_level(&shell->token_pointer, &shell->exec);
			assign_type(&shell->token_pointer);
			print_node(shell->token_pointer); //for testing
			shell->exit_code = execute(shell);
			builtins(shell); // for testing purposes
				//free_and_exit();
		}
		add_history(shell->user_input);
		delete_all_tokens(&shell->token_pointer);
		free(prompt);
		printf("exit code: %d\n", shell->exit_code);
	}
	free(shell->user_input); // replace with custom free function?
	return (shell->exit_code);
}
