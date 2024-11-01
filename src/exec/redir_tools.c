/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_tools.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklimchu <aklimchu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 12:04:25 by aklimchu          #+#    #+#             */
/*   Updated: 2024/10/30 15:06:57 by aklimchu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	check_for_input(t_shell *shell, t_token *token, int loop_count, int input_flag)
{
	t_token	*temp;

	temp = find_token_line(token, loop_count, REDIR_INPUT, "<\0");
	if (input_flag == 0 && (!temp || !temp->next || temp->next->type != INPUT))
		return (2);
	if (input_flag == 1 && (!temp || !temp->next || temp->next->type != INPUT))
		return (0);
	if (input_flag == 1)
		close(shell->exec->in);
	/* if (access(temp->next->line, R_OK) == -1 && errno == EACCES)
		printing(temp->next->line, "", ": Permission denied\n", 2);
	if (access(temp->next->line, F_OK) == -1 && errno == ENOENT)
		printing(temp->next->line, "", ": No such file or directory\n", 2); */
	shell->exec->in = open(temp->next->line, O_RDONLY);
	if (shell->exec->in == -1)
	{
		close_pipes_child(loop_count, &shell->exec); // free pids?
		free_and_exit(shell, 1);
		return (1);
	}
	check_for_input(shell, temp->next, loop_count, 1);
	return (0);
}

int	check_for_output(t_shell *shell, t_token *token, int loop_count, int output_flag)
{
	t_token	*temp;
	char	*outfile;
	
	temp = find_token(token, loop_count, REDIR_OUTPUT);
	if (output_flag == 0 && (!temp || !temp->next || temp->next->type != OUTPUT))
		return (2);
	if (output_flag == 1 && (!temp || !temp->next || temp->next->type != OUTPUT))
		return (0);
	if (output_flag == 1)
		close(shell->exec->out);
	outfile = temp->next->line;
	if ((outfile && non_existing_folder(outfile) == 0) || (outfile && outfile[0] == '\0'))
	{
		//printing(outfile, "", ": No such file or directory\n", 2); // what if $HOME?
		close_pipes_child(loop_count, &shell->exec); // free pids?
		free_and_exit(shell, 1);
		return (1);
	}
	else
	{
		if (ft_strncmp(temp->line, ">\0", 2) == 0)
			shell->exec->out = open(outfile, O_WRONLY | O_TRUNC | O_CREAT, 0777);
		else
			shell->exec->out = open(outfile, O_WRONLY | O_APPEND | O_CREAT, 0777);
		if (shell->exec->out == -1)
		{
			//is_directory(outfile, *shell->exec, 0, NULL);
			/* if (access(outfile, W_OK) == -1 && errno == EACCES)
				printing(outfile, "", ": Permission denied\n", 2); */
			close_pipes_child(loop_count, &shell->exec); // free pids?
			free_and_exit(shell, 1);
			return (1);
		}
	}
	check_for_output(shell, temp->next, loop_count, 1);
	return (0);
}