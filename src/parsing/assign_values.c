/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assign_values.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklimchu <aklimchu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 12:50:33 by jlehtone          #+#    #+#             */
/*   Updated: 2024/11/08 14:41:38 by aklimchu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	parse_here_doc(t_token *token)
{
	t_token	*temp;
	int		count;

	count = 0;
	temp = token;
	while (temp)
	{
		if (temp->type == REDIR_INPUT && ft_strncmp(temp->line, "<<\0", 3) == 0
			&& temp->next && temp->next->type == DELIM)
			count++;
		temp = temp->next;
	}
	return (count);
}

static void	check_type(t_token *temp, t_token *previous, int *comm_flag)
{
	if (ft_strncmp(temp->line, "|\0", 2) == 0)
	{
		temp->type = PIPE;
		*comm_flag = 0;
	}
	else if (ft_strncmp(temp->line, "<\0", 2) == 0 \
		|| ft_strncmp(temp->line, "<<\0", 3) == 0)
		temp->type = REDIR_INPUT;
	else if (ft_strncmp(temp->line, ">\0", 2) == 0 \
		|| ft_strncmp(temp->line, ">>\0", 3) == 0)
		temp->type = REDIR_OUTPUT;
	else if (ft_strncmp(previous->line, "<\0", 2) == 0)
		temp->type = INPUT;
	else if (ft_strncmp(previous->line, ">\0", 2) == 0 \
		|| ft_strncmp(previous->line, ">>\0", 3) == 0)
		temp->type = OUTPUT;
	else if (ft_strncmp(previous->line, "<<\0", 3) == 0)
		temp->type = DELIM;
}

void	assign_type(t_token **token)
{
	t_token	*temp;
	t_token	*previous;
	int		comm_flag;

	temp = *token;
	previous = temp;
	comm_flag = 0;
	while (temp)
	{
		check_type(temp, previous, &comm_flag);
		if (temp->type == 0 && comm_flag == 0)
		{
			temp->type = COMM;
			comm_flag = 1;
		}
		else if (temp->type == 0)
			temp->type = ARG;
		previous = temp;
		temp = temp->next;
	}
}

void	assign_level(t_token **token, t_exec **exec, t_shell *shell)
{
	int		level;
	t_token	*token_temp;

	level = 0;
	token_temp = *token;
	while (token_temp)
	{
		if (ft_strncmp(token_temp->line, "|\0", 2) == 0)
			level++;
		token_temp->level = level;
		token_temp = token_temp->next;
	}
	*exec = (t_exec *)malloc(sizeof(t_exec));
	if (*exec == NULL)
		error_printer(shell, "", MALLOC_FAIL, true);
	ft_memset(*exec, 0, sizeof(t_exec));
	(*exec)->pipe_num = level;
	(*exec)->here_doc_num = parse_here_doc(*token);
}
