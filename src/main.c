/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlehtone <jlehtone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 13:19:31 by aklimchu          #+#    #+#             */
/*   Updated: 2024/09/20 14:16:41 by jlehtone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static void	argc_check(int argc);

int	main(int argc, char *argv[], char *envp[])
{
	t_shell	shell;
	
	(void)argv;
	argc_check(argc);
	ft_memset(&shell, 0, sizeof(t_shell));
	if (copy_envp(&shell.envp_copy, envp) == 1)
	{
		perror("Malloc failed");
		return (1);
	}
	printf("%s\n", shell.envp_copy->line);
	shell.exit_code = read_input(&shell);
	// adding stuff to test
	// adding stuff
	
/* while (shell.envp_copy)
	{
		printf("%s\n", shell.envp_copy->line);
		shell.envp_copy = shell.envp_copy->next;
	} */
/* 	int i = 0;
	while (envp[i])
		printf("%s\n", envp[i++]); */
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