/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_files.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklimchu <aklimchu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 14:43:35 by aklimchu          #+#    #+#             */
/*   Updated: 2024/11/19 11:52:44 by aklimchu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	check_access_print(t_token *token, t_shell *shell);
static int	slash_count(char *path, int *last_slash);
static int	check_access_print_extra(t_token *token, t_shell *sh);

// The function checks all files passed by user in the prompt
// for access problems
void	check_all_files(t_token *token, t_exec *exec, int loop, t_shell *shell)
{
	t_token	*temp;
	int		i;

	i = 0;
	exec->error_node_index = -1;
	temp = token;
	while (temp && temp->level != loop)
		temp = temp->next;
	while (temp && temp->next && temp->level == loop)
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

// These two functions check the file for access problems
// and print out an error message if needed
static int	check_access_print(t_token *token, t_shell *shell)
{
	if (token->type == REDIR_INPUT && \
		access(token->next->line, R_OK) == -1 && errno == EACCES)
	{
		error_printer(shell, token->next->line, PERM_DENIED, false);
		return (1);
	}
	if (token->type == REDIR_INPUT && \
		access(token->next->line, F_OK) == -1 && errno == ENOENT)
	{
		error_printer(shell, token->next->line, NO_FILE_DIR, false);
		return (1);
	}
	if (token->type == REDIR_OUTPUT && token->next->line \
		&& token->next->line[0] == '\0')
	{
		error_printer(shell, token->next->line, NO_FILE_DIR, false);
		return (1);
	}
	if (token->type == REDIR_OUTPUT && access(token->next->line, W_OK) == -1 \
		&& errno == EACCES)
	{
		error_printer(shell, token->next->line, PERM_DENIED, false);
		return (1);
	}
	return (check_access_print_extra(token, shell));
}

static int	check_access_print_extra(t_token *token, t_shell *sh)
{
	if (token->type == REDIR_OUTPUT && is_directory_new(token->next->line) == 0)
	{
		error_printer(sh, token->next->line, IS_DIR, false);
		return (1);
	}
	if (token->type == REDIR_OUTPUT && out_folder(token->next->line, sh) == 1)
	{
		error_printer(sh, token->next->line, NO_FILE_DIR, false);
		return (1);
	}
	if (token->type == REDIR_OUTPUT && out_folder(token->next->line, sh) == 2)
	{
		error_printer(sh, token->next->line, IS_DIR, false);
		return (1);
	}
	if (token->type == REDIR_OUTPUT && out_folder(token->next->line, sh) == 3)
	{
		error_printer(sh, token->next->line, PERM_DENIED, false);
		return (1);
	}
	return (0);
}

// The function checks if the path provided by user is a folder
int	out_folder(char *path, t_shell *shell)
{
	int		last_slash;
	char	*folder_path;
	int		i;

	last_slash = 0;
	i = slash_count(path, &last_slash);
	if (!i)
		return (0);
	if (last_slash == i - 1)
		return (2);
	if (last_slash == 0)
		return (3);
	folder_path = ft_substr(path, 0, last_slash);
	if (!folder_path)
		error_printer(shell, "", MALLOC_FAIL, true);
	if (is_directory_new(folder_path) == 1)
	{
		free(folder_path);
		return (1);
	}
	free(folder_path);
	return (0);
}

// The function counts the number of slashes in the path
static int	slash_count(char *path, int *last_slash)
{
	int	count;
	int	i;

	i = 0;
	count = 0;
	while (path[i])
	{
		if (path[i] == '/')
			count++;
		i++;
	}
	if (count == 0)
		return (0);
	i = 0;
	while (path[i])
	{
		if (path[i++] == '/')
			*last_slash = i;
	}
	return (i);
}
