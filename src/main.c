//42 header

#include "../inc/minishell.h"

static void	argc_check(int argc);

int	main(int argc, char *argv[], char *envp[])
{
	t_shell	shell;
	
	(void)argv;
	argc_check(argc);
	ft_memset(&shell, 0, sizeof(t_shell));
	shell.user_input = NULL;
	if (copy_envp(&shell, &shell.envp_copy, envp) == 1)
		error_printer(&shell, "", MALLOC_FAIL, true);
	shell.exit_code = read_input(&shell);
	delete_envp(&shell.envp_copy);
	return (0);
}

static void	argc_check(int argc)
{
	if (argc > 1)
	{
		printf("Correct input to start the shell: \n");
		printf("./minishell\n");
		printf("No extra arguments needed\n");
		exit(1);
	}
}