/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_tools_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklimchu <aklimchu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 10:35:21 by aklimchu          #+#    #+#             */
/*   Updated: 2024/11/13 13:09:34 by aklimchu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	get_index(t_shell *shell, t_token *token, t_token *temp, int loop);

// The function looks for the token containing a path to output file 
// (not using recursion) and attempts to open it
int	check_out_no_recur(t_shell *s, t_token *tok, int loop, int error_node)
{
	t_token	*temp;
	char	*outfile;

	if (error_node == -1)
		return (0);
	temp = find_token_index(tok, loop, REDIR_OUTPUT, error_node);
	if (!temp || !temp->next || temp->next->type != OUTPUT)
		return (0);
	outfile = temp->next->line;
	if (outfile && outfile[0] == '\0')
	{
		close_pipes_child(loop, &s->exec);
		free_and_exit(s, 1);
		return (1);
	}
	else
		return (open_file(s, temp, outfile, loop));
}

// The function attempts to open the file
int	open_file(t_shell *shell, t_token *temp, char *outfile, int loop)
{
	if (ft_strncmp(temp->line, ">\0", 2) == 0)
		shell->exec->out = open(outfile, O_WRONLY | O_TRUNC | O_CREAT, 0777);
	else
		shell->exec->out = open(outfile, O_WRONLY | O_APPEND | O_CREAT, 0777);
	if (shell->exec->out == -1)
	{
		close_pipes_child(loop, &shell->exec);
		free_and_exit(shell, 1);
		return (1);
	}
	return (0);
}

// The function returns the number of here_doc tokens in user input
int	check_for_here_doc(t_shell *shell, t_token *token, int loop)
{
	t_token	*temp;

	temp = token;
	while (temp && temp->level != loop)
		temp = temp->next;
	if (check_out_no_recur(shell, temp, loop, \
		shell->exec->error_node_index) == 1)
		return (-3);
	temp = token;
	while (temp && temp->level != loop)
		temp = temp->next;
	return (get_index(shell, token, temp, loop));
}

// The function calculates the number of here_doc tokens
static int	get_index(t_shell *shell, t_token *token, t_token *temp, int loop)
{
	t_token	*res;
	int		here_doc_index;

	res = NULL;
	while (temp && temp->next && temp->level == loop)
	{
		if (temp->type == REDIR_INPUT)
		{
			res = temp;
			if (ft_strlen(res->line) == 1)
				check_file_access(shell, res->next->line, loop);
		}
		temp = temp->next;
	}
	if (!res || ft_strncmp(res->line, "<<\0", 3))
		return (-2);
	here_doc_index = 0;
	temp = token;
	while (temp && temp->next && temp->level <= loop)
	{
		if (temp->type == REDIR_INPUT && ft_strncmp(temp->line, "<<\0", 3) == 0)
			here_doc_index++;
		temp = temp->next;
	}
	return (here_doc_index - 1);
}
