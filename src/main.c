/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlehtone <jlehtone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 13:19:31 by aklimchu          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2024/09/20 13:53:36 by aklimchu         ###   ########.fr       */
=======
/*   Updated: 2024/09/20 13:02:38 by jlehtone         ###   ########.fr       */
>>>>>>> remotes/origin/main
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static void	argc_check(int argc);

static void	printing_tests(t_shell shell);

int	main(int argc, char *argv[], char *envp[])
{
	t_shell	shell;
	
	(void)argv;
	argc_check(argc);
<<<<<<< HEAD
	if (copy_envp(&shell, &shell.envp_copy, envp) == 1)
=======
	ft_memset(&shell, 0, sizeof(t_shell));
	if (copy_envp(&shell.envp_copy, envp) == 1)
>>>>>>> remotes/origin/main
	{
		perror("Malloc failed");
		return (1);
	}
<<<<<<< HEAD
	printing_tests(shell);
=======
	printf("%s\n", shell.envp_copy->line);
	shell.exit_code = read_input(&shell);
	
/* while (shell.envp_copy)
	{
		printf("%s\n", shell.envp_copy->line);
		shell.envp_copy = shell.envp_copy->next;
	} */
/* 	int i = 0;
	while (envp[i])
		printf("%s\n", envp[i++]); */
>>>>>>> remotes/origin/main
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