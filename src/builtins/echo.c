/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlehtone <jlehtone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 12:02:54 by aklimchu          #+#    #+#             */
/*   Updated: 2024/11/01 13:54:22 by jlehtone         ###   ########.fr       */
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

// prints each ARG-type node, until it hits an IO. 
// prints spaces between ARGS, unless it hasnt printed yet
static void	echo_printing_loop(t_token *temp)
{
	int	printed;

	printed = false;
	while (temp)
	{
		if (temp->type == PIPE) //maybe not extensive enough
			break ;
		if (temp->type == ARG)
		{
			printf("%s", temp->line);
			printed = true;
		}
		if (temp->next && temp->next->type == ARG && printed == true)
			printf(" ");
		temp = temp->next;
	}
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
	echo_printing_loop(temp);
	if (newline == true)
		printf("\n");
	return (SUCCESS);
}
