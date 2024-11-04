//42 header

#include "../../inc/minishell.h"

void	handle_input(t_shell *shell)
{
	tokenize_input(shell);
	expander(shell);
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

static char	*create_prompt(t_shell *shell)
{
	char *prompt;

	free(shell->pwd);
	shell->pwd = get_pwd(shell->home, shell);
	shell->envp_str = NULL;
	prompt = ft_strjoin_four(shell->uname, ":", shell->pwd, "$ ");
	if (prompt == NULL)
		error_printer(shell, MALLOC_FAIL, true);
	return (prompt);
}

int read_input(t_shell *shell)
{
	char	*prompt;

	shell->pwd = get_pwd(shell->home, shell);
	while (true) 
	{
		prompt = create_prompt(shell);
		set_up_signals(shell);
		shell->user_input = readline(prompt);
		if (shell->user_input == NULL)
			null_signal(shell);
		if (input_error_check(shell) == SUCCESS)
			handle_input(shell);
		add_history(shell->user_input);
		free(shell->user_input); // is it correct place?
		free(prompt);
		delete_all_tokens(&shell->token_pointer);
	}
	//free(shell->user_input); // replace with custom free function?
	//printf("exit code: %d\n", shell->exit_code);
	return (shell->exit_code);
}
