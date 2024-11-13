/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklimchu <aklimchu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 12:51:29 by aklimchu          #+#    #+#             */
/*   Updated: 2024/11/13 10:38:00 by aklimchu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	check_no_command(t_token *token, int loop_count);
static void	empty_path(t_envp **path_to_curr, t_shell *shell);
static void	call_execve(t_shell **shell, char *path, int loop);
static void	free_temp(t_shell **shell);

// The function executes the child process
void	child_process(t_shell **shell, int loop_count)
{
	t_exec	*exec;
	char	*path;
	int		exit_code;

	exec = (*shell)->exec;
	if (check_no_command((*shell)->token_pointer, loop_count) == 1)
	{
		close_pipes_child(loop_count, &exec);
		free_and_exit(*shell, false);
	}
	get_input_and_output(shell, loop_count);
	close_pipes_child(loop_count, &exec);
	if (if_builtin(*shell, loop_count) == 0)
	{
		exit_code = exec_builtins(*shell, loop_count);
		free_and_exit(*shell, exit_code);
	}
	check_param(*shell, loop_count);
	if (!(*shell)->path)
		empty_path(&(*shell)->path, *shell);
	path = check_path((*shell)->path, (*shell)->exec->param, *shell);
	if (path == NULL)
		error_printer(*shell, "", MALLOC_FAIL, true);
	call_execve(shell, path, loop_count);
}

// The function checks if there is a token with command name
static int	check_no_command(t_token *token, int loop_count)
{
	t_token	*temp;

	temp = token;
	while (temp && temp->level != loop_count)
		temp = temp->next;
	while (temp && temp->level == loop_count)
	{
		if (temp->type == COMM)
			return (0);
		temp = temp->next;
	}
	return (1);
}

// The function creates an empty PATH is it earlier unset
// in the environment
static void	empty_path(t_envp **path_to_curr, t_shell *shell)
{
	char	*pwd;
	t_envp	*new;

	pwd = (char *)malloc(BUFF_SIZE * sizeof(char));
	if (pwd == NULL)
		error_printer(shell, "", MALLOC_FAIL, true);
	if (getcwd(pwd, BUFF_SIZE) == NULL)
	{
		free_str(&pwd);
		error_printer(shell, "", GETCWD_FAIL, true);
	}
	new = ft_lstnew_envp_no_strdup(pwd);
	ft_lstadd_back_envp(path_to_curr, new);
}

// The function executes the command using execve
static void	call_execve(t_shell **shell, char *path, int loop)
{
	if (find_token_line((*shell)->token_pointer, loop, COMM, "./minishell"))
		change_shlvl(&((*shell)->envp_copy), 1, *shell);
	(*shell)->envp_str = envp_to_arr((*shell)->envp_copy);
	if ((*shell)->envp_str == NULL)
		error_printer(*shell, "", MALLOC_FAIL, true);
	free_temp(shell);
	if (execve(path, (*shell)->exec->param, (*shell)->envp_str) == -1)
	{
		free_double_arr(&(*shell)->envp_str);
		error_printer(*shell, (*shell)->exec->param[0], PERM_DENIED_COMM, true);
	}
}

// The function frees temporary allocated memory
static void	free_temp(t_shell **shell)
{
	free_str(&(*shell)->user_input);
	free_str(&(*shell)->home);
	delete_envp(&(*shell)->path);
	delete_all_tokens(&(*shell)->token_pointer);
	delete_envp(&(*shell)->envp_copy);
}
