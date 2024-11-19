/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklimchu <aklimchu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 14:32:40 by aklimchu          #+#    #+#             */
/*   Updated: 2024/11/19 13:21:02 by aklimchu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	free_pipes(t_exec **exec);
static void	free_here_doc_pipes(t_exec **exec);

// The function frees the temporary struct "exec"
int	free_exec(t_exec **exec)
{
	if (!(*exec))
		return (1);
	if ((*exec)->pid)
	{
		free((*exec)->pid);
		(*exec)->pid = NULL;
	}
	free_pipes(exec);
	free_here_doc_pipes(exec);
	free_str(&(*exec)->new_path);
	free_double_arr(&(*exec)->param);
	if (*exec)
	{
		free(*exec);
		*exec = NULL;
	}
	return (1);
}

// The function frees the allocated pipes
static void	free_pipes(t_exec **exec)
{
	int	i;

	if ((*exec)->pipe_num > 0 && (*exec)->pipe)
	{
		i = 0;
		while (i < (*exec)->pipe_num)
		{
			if ((*exec)->pipe[i])
			{
				free((*exec)->pipe[i]);
				(*exec)->pipe[i] = NULL;
			}
			i++;
		}
		free((*exec)->pipe);
		(*exec)->pipe = NULL;
	}
}

// The function frees the allocated here_doc pipes
static void	free_here_doc_pipes(t_exec **exec)
{
	int	i;

	if ((*exec)->here_doc_num > 0 && (*exec)->here_doc_pipe)
	{
		i = 0;
		while (i < (*exec)->here_doc_num)
		{
			if ((*exec)->here_doc_pipe[i])
			{
				free((*exec)->here_doc_pipe[i]);
				(*exec)->here_doc_pipe[i] = NULL;
			}
			i++;
		}
		free((*exec)->here_doc_pipe);
		(*exec)->here_doc_pipe = NULL;
	}
}

// The function closes open pipes (called in a child process)
void	close_pipes_child(int loop_count, t_exec **exec)
{
	int		i;

	i = 0;
	while (i < (*exec)->pipe_num && i < loop_count + 1)
	{
		close((*exec)->pipe[i][0]);
		close((*exec)->pipe[i][1]);
		i++;
	}
	i = 0;
	while (i < (*exec)->here_doc_num)
	{
		close((*exec)->here_doc_pipe[i][0]);
		i++;
	}
	(*exec)->here_doc_closed = true;
}
