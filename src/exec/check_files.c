/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_files.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklimchu <aklimchu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 14:43:35 by aklimchu          #+#    #+#             */
/*   Updated: 2024/10/30 15:31:36 by aklimchu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	check_access_print(t_token *token);

void	check_file_access(t_shell *shell, char	*path, int loop_count)
{
	if (access(path, R_OK) == -1 && errno == EACCES)
	{
		//printing(path, "", ": Permission denied\n", 2);
		close_pipes_child(loop_count, &shell->exec); // free pids?
		exit(1);
	}
	if (access(path, F_OK) == -1 && errno == ENOENT)
	{
		//printing(path, "", ": No such file or directory\n", 2);
		close_pipes_child(loop_count, &shell->exec); // free pids?
		exit(1);
	}
}

void	check_all_files(t_token *token, int loop_count)
{
	t_token	*temp;
	
	temp = token;
	while (temp && temp->level != loop_count)
		temp = temp->next;
	while (temp && temp->next && temp->level == loop_count)
	{
		if (((temp->type == REDIR_INPUT && ft_strlen(temp->line) == 1) || \
			temp->type == REDIR_OUTPUT) && check_access_print(temp) == 1)
			break ;
		temp = temp->next;
	}
}

static int	check_access_print(t_token *token)
{
	if (token->type == REDIR_INPUT && \
		access(token->next->line, R_OK) == -1 && errno == EACCES)
	{
		printing(token->next->line, "", ": Permission denied\n", 2);
		return (1);
	}
	if (token->type == REDIR_INPUT && \
		access(token->next->line, F_OK) == -1 && errno == ENOENT)
	{
		printing(token->next->line, "", ": No such file or directory\n", 2);
		return (1);
	}
	if (token->type == REDIR_OUTPUT && token->next->line && token->next->line[0] == '\0')
	{
		printing(token->next->line, "", ": No such file or directory\n", 2);
		return (1);
	}
	if (token->type == REDIR_OUTPUT && access(token->next->line, W_OK) == -1 \
		&& errno == EACCES)
	{
		printing(token->next->line, "", ": Permission denied\n", 2);
		return (1);
	}
	if (token->type == REDIR_OUTPUT && is_directory_new(token->next->line) == 0)
	{
		printing(token->next->line, "", ": Is a directory\n", 2);
		return (1);
	}
	return (0);
}