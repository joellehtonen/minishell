//42 header

#include "../../inc/minishell.h"

static void	check_empty(t_token *token, int loop_count);

static char	*get_exec_path(t_envp *envp_copy, char *command, char **param, t_exec exec);

char	**check_param(t_shell *shell, int loop_count)
{
	char	**param;
	//int		word_num;

	check_empty(shell->token_pointer, loop_count);
	
	//word_num = count_param(str);
	param = param_to_arr(shell->token_pointer, loop_count);
	if (param == NULL)
		perror("malloc() failed");
	//free(str_new); 
	return (param);
}

char	*check_path(t_envp *paths, char **param, t_exec exec)
{
	char	*exec_path;
	char	*command;

	command = param[0];
	if (ft_strrchr(command, '/'))
	{
		is_directory(command, exec, -1, param);
		check_command_access(param, exec);
		return (command);
	}
	if (paths == NULL)
	{
		printing(command, "", ": No such file or directory\n", 2);
/* 		free_all(param, NULL, NULL, &fd.pid);
 */		exit(127);
	}
	exec_path = get_exec_path(paths, command, param, exec);
	return (exec_path);
}

static void	check_empty(t_token *token, int loop_count)
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
		exit(127);
	}
}

static char	*get_exec_path(t_envp *paths, char *command, char **param, t_exec exec)
{
	char	*exec_path;

	(void)param;
	(void)exec;
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
/* 	free_all(path, NULL, NULL, &fd.null);
 */	if (exec_path == NULL)
	{
		printing(command, "", ": command not found\n", 2);
		/* free_all(NULL, param, NULL, &fd.pid); */
		exit(127);
	}
	return (exec_path);
}