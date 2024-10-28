/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlehtone <jlehtone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 12:02:54 by aklimchu          #+#    #+#             */
/*   Updated: 2024/10/11 10:14:13 by jlehtone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	check_newline(char *content)
{
	int	index;

	index = 0;
	if (content[index] == '-')
		index++;
	else
		return (true);
	while (content[index] == 'n')
		index++;
	if (content[index] == '\0')
		return (false);
	else
		return (true);
}

// I'm assuming that the exec function is already pointing to the node containing COMM "echo"
int echo(t_shell *shell, t_token *echo_pointer)
{
	int		newline;
	t_token	*temp;
	
	(void)shell;
	newline = true;
	temp = echo_pointer->next;
	if (temp)
	{
		newline = check_newline(temp->line);
		if (newline == false)
			temp = temp->next;
	}
	while (temp && temp->type == ARG)
	{
		printf("%s", temp->line);
		if (temp->next && temp->next->type == ARG)
			printf(" ");
		temp = temp->next;
	}
	if (newline == true)
		printf("\n");
	return (SUCCESS);
}
