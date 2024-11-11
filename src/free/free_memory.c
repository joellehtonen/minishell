/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_memory.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklimchu <aklimchu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 13:16:06 by jlehtone          #+#    #+#             */
/*   Updated: 2024/11/11 10:08:42 by aklimchu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	free_shell(t_shell **shell, int free_envp)
{
	free_str(&(*shell)->user_input);
	free_exec(&(*shell)->exec);
	free_double_arr(&(*shell)->envp_str);
	free_str(&(*shell)->home);
	delete_envp(&(*shell)->path);
	delete_all_tokens(&(*shell)->token_pointer);
	if (free_envp == true)
		delete_envp(&(*shell)->envp_copy);
}

void	free_double_arr(char ***arr)
{
	int		i;

	i = 0;
	if (*arr)
	{
		while ((*arr)[i])
			free_str(&(*arr)[i++]);
		free(*arr);
		*arr = NULL;
	}
}

char	**free_double_arr_part(char ***arr, int count)
{
	int		i;

	i = 0;
	if (*arr)
	{
		while ((*arr)[i] && i <= count)
			free_str(&(*arr)[i++]);
		free(*arr);
		*arr = NULL;
	}
	return (NULL);
}

int	free_two_str(char **str1, char **str2)
{
	if (*str1)
	{
		free(*str1);
		*str1 = NULL;
	}
	if (*str2)
	{
		free(*str2);
		*str2 = NULL;
	}
	return (1);
}

int	free_str(char **str1)
{
	if (*str1)
	{
		free(*str1);
		*str1 = NULL;
	}
	return (1);
}
