/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlehtone <jlehtone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 12:02:54 by aklimchu          #+#    #+#             */
/*   Updated: 2024/11/18 17:03:50 by jlehtone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int		check_newline(char *content);
static t_token	*skip_flags(t_token *temp, int valid_flag);
static void		echo_printing_loop(t_token *temp);

// executes echo builtin
int	echo(t_shell *shell, t_token *echo_pointer)
{
	int		no_newline;
	int		valid_flag;
	t_token	*temp;

	(void)shell;
	no_newline = false;
	valid_flag = false;
	temp = echo_pointer->next;
	if (temp)
	{
		no_newline = check_newline(temp->line);
		if (no_newline == true)
		{
			valid_flag = true;
			temp = skip_flags(temp, valid_flag);
		}
	}
	echo_printing_loop(temp);
	if (no_newline == false)
		printf("\n");
	return (SUCCESS);
}

//checks for the newline flag -n
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

static t_token	*skip_flags(t_token *temp, int valid_flag)
{
	while (valid_flag == true)
	{
		if (temp)
		{
			valid_flag = check_newline(temp->line);
			if (valid_flag == true)
				temp = temp->next;
		}
		else
			break ;
	}
	return (temp);
}

// prints each ARG-type node, until it hits an I/O operator
// prints spaces between ARGS, unless it hasnt printed yet
static void	echo_printing_loop(t_token *temp)
{
	int	printed;

	printed = false;
	while (temp)
	{
		if (temp->type == PIPE)
			break ;
		if (temp->type == ARG && ft_strlen(temp->line) != 0)
		{
			printf("%s", temp->line);
			printed = true;
		}
		if (temp->next && temp->next->type == ARG && printed == true)
		{
			if (ft_strlen(temp->line) != 0
				|| (ft_strlen(temp->line) == 0 && temp->expanded == false))
			{
				printf(" ");
			}
		}
		temp = temp->next;
	}
}
