/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_tools.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklimchu <aklimchu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 12:04:25 by aklimchu          #+#    #+#             */
/*   Updated: 2024/11/18 14:54:19 by aklimchu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// The function looks for the token containing a path to input file 
// and attempts to open it
int	check_for_input(t_shell *shell, t_token *token, int loop, int input)
{
	t_token	*temp;

	temp = find_token_line(token, loop, REDIR_INPUT, "<\0");
	if (input == 0 && (!temp || !temp->next || temp->next->type != INPUT))
		return (2);
	if (input == 1 && (!temp || !temp->next || temp->next->type != INPUT))
		return (0);
	if (input == 1)
		close(shell->exec->in);
	shell->exec->in = open(temp->next->line, O_RDONLY);
	if (shell->exec->in == -1)
	{
		close_pipes_child(loop, &shell->exec);
		free_and_exit(shell, 1);
		return (1);
	}
	return (check_for_input(shell, temp->next, loop, 1));
}

// The function looks for the token containing a path to output file 
// and attempts to open it
int	check_for_output(t_shell *shell, t_token *token, int loop, int output)
{
	t_token	*temp;
	char	*outfile;

	temp = find_token(token, loop, REDIR_OUTPUT);
	if (output == 0 && (!temp || !temp->next || temp->next->type != OUTPUT))
		return (2);
	if (output == 1 && (!temp || !temp->next || temp->next->type != OUTPUT))
		return (0);
	if (output == 1)
		close(shell->exec->out);
	outfile = temp->next->line;
	if ((outfile && out_folder(outfile, shell)) || \
		(outfile && outfile[0] == '\0'))
	{
		close_pipes_child(loop, &shell->exec);
		free_and_exit(shell, 1);
		return (1);
	}
	else if (open_file(shell, temp, outfile, loop) == 1)
		return (1);
	return (check_for_output(shell, temp->next, loop, 1) == 1);
}
