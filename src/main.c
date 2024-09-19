/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklimchu <aklimchu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 13:19:31 by aklimchu          #+#    #+#             */
/*   Updated: 2024/09/19 13:40:01 by aklimchu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	main(int argc, char *argv[], char *envp[])
{
	(void)argv;
	(void)envp;
	
	if (argc > 1)
	{
		printf("Correct input to start the shell: \n");
		printf("./minishell\n");
		printf("No extra arguments needed\n");
		return (0);
	}
	return (0);
}