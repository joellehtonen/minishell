//42 header

#include "../../inc/minishell.h"

static void	check_empty(t_token *token, int loop_count, t_shell *shell);

static char	*get_exec_path(t_envp *envp_copy, char *command, char **param, t_shell *shell);

char	**check_param(t_shell *shell, int loop_count)
{
	char	**param;

	check_empty(shell->token_pointer, loop_count, shell);
	param = param_to_arr(shell->token_pointer, loop_count);
	return (param);
}

char	*check_path(t_envp *paths, char **param, t_shell *shell)
{
	char	*exec_path;
	char	*command;

	command = param[0];
	if (ft_strrchr(command, '/'))
	{
		is_directory(command, param, shell);
		check_command_access(param, shell);
		return (command);
	}
	if (paths == NULL)
	{
		printing(command, "", ": No such file or directory\n", 2);
		free_double_arr(&param);
		free_and_exit(shell, 127);
	}
	exec_path = get_exec_path(paths, command, param, shell);
	return (exec_path);
}

static void	check_empty(t_token *token, int loop_count, t_shell *shell)
{
	t_token	*temp;
	
	temp = token;
	while (temp && temp->level != loop_count)
		temp = temp->next;
	while (temp && temp->level == loop_count)
	{
		if (temp->type == COMM)
			break ;
		temp = temp->next;
	}
	if (!temp || temp->level != loop_count)
		return ;
	if (temp->line[0] == '\0' || temp->line[0] == ' ')
	{
		printing(temp->line, "", ": command not found\n", 2);
		free_and_exit(shell, 127);
	}
}

static char	*get_exec_path(t_envp *paths, char *command, char **param, t_shell *shell)
{
	char	*exec_path;

	exec_path = NULL;
	while (paths)
	{
		exec_path = ft_strjoin_four(paths->line, "/", "", command);
		if (exec_path && access(exec_path, F_OK) == 0)
			break ;
		free(exec_path);
		exec_path = NULL;
		paths = paths->next;
	}
	if (exec_path == NULL)
	{
		printing(command, "", ": command not found\n", 2);
		free_double_arr(&param);
		free_and_exit(shell, 127);
	}
	return (exec_path);
}