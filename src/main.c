/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklimchu <aklimchu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 13:19:31 by aklimchu          #+#    #+#             */
/*   Updated: 2024/09/25 13:18:37 by aklimchu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../inc/minishell.h"

static void	argc_check(int argc);

/* static void	get_home(t_shell *shell);
 */
static void	printing_tests(t_shell shell);

int	main(int argc, char *argv[], char *envp[])
{
	t_shell	shell;
	
	(void)argv;
	argc_check(argc);
	ft_memset(&shell, 0, sizeof(t_shell));
	shell.user_input = NULL;
	if (copy_envp(&shell, &shell.envp_copy, envp) == 1)
	{
		perror("Malloc failed");
		//free_and_exit();
	}
	shell.exit_code = read_input(&shell);
	printing_tests(shell); // for testing purposes
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

/* static void	get_home(t_shell *shell)
{
	shell->home = (char *)malloc(BUFF_SIZE * sizeof(char));
	if (shell->home == NULL)
	{
		perror("malloc error");
		exit(1);
	}
	if (getcwd(shell->home, BUFF_SIZE) == NULL)
	{
		perror("getcwd error");
		exit(1);
	}
} */

static void	printing_tests(t_shell shell)
{
	printf("Our ENVP\n");
	while (shell.envp_copy)
	{
		printf("%s\n", shell.envp_copy->line);
		shell.envp_copy = shell.envp_copy->next;
	}
	printf("\n\n\n\nOur PATH divided into nodes\n");
	while (shell.path)
	{
		printf("%s\n", shell.path->line);
		shell.path = shell.path->next;
	}
	printf("\n\n\n\nExtracted username\n");
	printf("%s\n", shell.uname);
}