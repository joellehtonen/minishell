/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_new.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlehtone <jlehtone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 12:02:54 by aklimchu          #+#    #+#             */
/*   Updated: 2024/10/03 17:15:48 by jlehtone         ###   ########.fr       */
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
		return (false);
	while (content[index] == 'n')
		index++;
	if (content[index] == '\0')
		return (true);
	else
		return (false);
}

// I'm assuming that the exec function is already pointing to the node containing COMM "echo"
int echo(t_shell *shell, t_token *echo_pointer)
{
	int		newline;
	t_token	*temp;
	
	newline = true;
	temp = echo_pointer->next;
	if (temp && temp->type == FLAG)
	{
		newline = check_newline(temp->line);
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
