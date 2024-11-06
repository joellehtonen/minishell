/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_files.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklimchu <aklimchu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 14:43:35 by aklimchu          #+#    #+#             */
/*   Updated: 2024/11/05 11:22:53 by aklimchu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	check_access_print(t_token *token, t_shell *shell);

static int slash_count(char *path);

static int	check_access_print_extra(t_token *token, t_shell *shell);

void	check_all_files(t_token *token, t_exec *exec, int loop_count, t_shell *shell)
{
	t_token	*temp;
	int		i;
	
	i = 0;
	exec->error_node_index = -1;
	temp = token;
	while (temp && temp->level != loop_count)
		temp = temp->next;
	while (temp && temp->next && temp->level == loop_count)
	{
		if (((temp->type == REDIR_INPUT && ft_strlen(temp->line) == 1) || \
			temp->type == REDIR_OUTPUT) && check_access_print(temp, shell) == 1)
		{
			if (temp->type == REDIR_INPUT && ft_strlen(temp->line) == 1)
				exec->error_node_index = i;
			break ;
		}
		temp = temp->next;
		i++;
	}
}

static int	check_access_print(t_token *token, t_shell *shell)
{
	if (token->type == REDIR_INPUT && \
		access(token->next->line, R_OK) == -1 && errno == EACCES)
	{
		//printing(token->next->line, "", ": Permission denied\n", 2);
		error_printer(shell, token->next->line, PERM_DENIED, false);
		return (1);
	}
	if (token->type == REDIR_INPUT && \
		access(token->next->line, F_OK) == -1 && errno == ENOENT)
	{
		//printing(token->next->line, "", ": No such file or directory\n", 2);
		error_printer(shell, token->next->line, NO_FILE_DIR, false);
		return (1);
	}
	if (token->type == REDIR_OUTPUT && token->next->line && token->next->line[0] == '\0')
	{
		//printing(token->next->line, "", ": No such file or directory\n", 2);
		error_printer(shell, token->next->line, NO_FILE_DIR, false);
		return (1);
	}
	if (token->type == REDIR_OUTPUT && access(token->next->line, W_OK) == -1 \
		&& errno == EACCES)
	{
		error_printer(shell, token->next->line, PERM_DENIED, false);
		//printing(token->next->line, "", ": Permission denied\n", 2);
		return (1);
	}
	return (check_access_print_extra(token, shell));
}

static int	check_access_print_extra(t_token *token, t_shell *shell)
{
	if (token->type == REDIR_OUTPUT && is_directory_new(token->next->line) == 0)
	{
		//printing(token->next->line, "", ": Is a directory\n", 2);
		error_printer(shell, token->next->line, IS_DIR, false);
		return (1);
	}
	if (token->type == REDIR_OUTPUT && check_output_folder(token->next->line) == 1)
	{
		//printing(token->next->line, "", ": No such file or directory\n", 2);
		error_printer(shell, token->next->line, NO_FILE_DIR, false);
		return (1);
	}
	if (token->type == REDIR_OUTPUT && check_output_folder(token->next->line) == 2)
	{
		//printing(token->next->line, "", ": Is a directory\n", 2);
		error_printer(shell, token->next->line, IS_DIR, false);
		return (1);
	}
	if (token->type == REDIR_OUTPUT && check_output_folder(token->next->line) == 3)
	{
		//printing(token->next->line, "", ": Permission denied\n", 2);
		error_printer(shell, token->next->line, PERM_DENIED, false);
		return (1);
	}
	return (0);
}

int	check_output_folder(char *path)
{
	int		last_slash;
	int		i;
	char	*folder_path;

	if (slash_count(path) == 0)
		return (0);
	i = 0;
	last_slash = 0;
	while (path[i])
	{
		if (path[i] == '/')
			last_slash = i;
		i++;
	}
	if (last_slash == i - 1)
		return (2);
	if (last_slash == 0)
		return (3);
	folder_path = ft_substr(path, 0, last_slash);
	if (is_directory_new(folder_path) == 1)
	{
		free(folder_path);
		return (1);
	}
	free(folder_path);
	return (0);
}

static int slash_count(char *path)
{
	int count;

	count = 0;
	while (*path)
	{
		if (*path == '/')
			count++;
		path++;
	}
	return (count);
}