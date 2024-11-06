//42 header

#include "../../inc/minishell.h"

static int	check_arg(t_shell *shell, t_token *arg);
static int	cd_no_arg(t_shell *shell);
static int	access_new_path(t_shell *shell, t_token *arg);

int	cd_exec(t_shell *shell, t_token *cd, int loop_count)
{
	t_token	*arg;
	int		arg_error;
	
	if (count_nodes_type(cd, ARG, loop_count) > 1)
   	{
		error_printer(shell, "", "cd: too many arguments\n", true);
		return (1);
	}
	arg = find_token(cd, loop_count, ARG);
	arg_error = check_arg(shell, arg);
	if (arg_error != 2)
		return (arg_error);
	shell->new_path = get_new_path(shell, arg);
	if (shell->new_path == NULL)
		return (1);
	return (access_new_path(shell, arg));
}

static int	check_arg(t_shell *shell, t_token *arg)
{
	if (!arg || ft_strlen(arg->line) == 0)
		return (cd_no_arg(shell));
	if (ft_strlen(arg->line) == 0)
		return (0);
	/* if (arg->line[0] == '/' && ft_strlen(arg->line) > 1 && arg->line[1] != '/')
	{
		printing("cd: ", arg->line, ": No such file or directory\n", 2);
		//free_and_exit();
		return (1);
	} */
	if (ft_strncmp(arg->line, "~", 1) != 0 && is_file(arg->line) == 0)
	{
		//printing("cd: ", arg->line, ": Not a directory\n", 2);
		//free_and_exit(shell, true);
		error_printer(shell, arg->line, NOT_DIR, true);
		return (1);
	}
	return (2);
}

static int cd_no_arg(t_shell *shell)
{
	if (shell->home == NULL)
	{
		error_printer(shell, "", "cd: HOME not set\n", true);
		return (1);
	}
	if (is_directory_new(shell->home) == 1)
	{
		//printing("cd: ", shell->home, ": No such file or directory\n", 2);
		//free_and_exit(shell, true);
		error_printer(shell, shell->home, NO_FILE_DIR, true);
		return (1);
	}
	if (chdir(shell->home) == -1)
	{
		error_printer(shell, "", CHDIR_ERROR, true);
		return (1);
	}
	else
		update_pwd(&shell->envp_copy, shell);
	return (0);
}

static int	access_new_path(t_shell *shell, t_token *arg)
{
	if (access(shell->new_path, F_OK) == -1 && errno == ENOENT)
	{
		if (arg->line[0] == '~')
		{
			//printing("cd: ", new_path, ": No such file or directory\n", 2);
			error_printer(shell, shell->new_path, NO_FILE_DIR, true);
		}
		else
		{
			//printing("cd: ", arg->line, ": No such file or directory\n", 2);
			error_printer(shell, arg->line, NO_FILE_DIR, true);
		}
		//free_and_exit(shell, true);
		return (1);
	}
	//set pwd in env
	if (chdir(shell->new_path) == -1)
	{
	   	error_printer(shell, "", CHDIR_ERROR, true);
		return (1);
	}
	else
		update_pwd(&shell->envp_copy, shell);
	return (0);
}