//42 header

#include "../../inc/minishell.h"

static int	check_arg(t_shell *shell, t_token *arg);

static int cd_no_arg(t_shell *shell);

static char	*get_new_path(t_shell *shell, t_token *arg);

static int	access_new_path(t_shell *shell, t_token *arg, char *new_path);

int	cd_exec(t_shell *shell, t_token *cd, int loop_count)
{
	char	*new_path;
	t_token	*arg;
	int		arg_error;
	
	if (count_nodes_type(cd, ARG, loop_count) > 1)
	{
		printing("cd", ": too many arguments\n", "", 2);
		//free_and_exit();
		return (1);
	}
	arg = find_token(cd, loop_count, ARG);
	arg_error = check_arg(shell, arg);
	if (arg_error != 2)
		return(arg_error);
	new_path = get_new_path(shell, arg);
	if (new_path == NULL)
		return (1);
	if (access_new_path(shell, arg, new_path) == 1)
		return (1);
	return (0);
}

static int	check_arg(t_shell *shell, t_token *arg)
{
	if (!arg)
		return (cd_no_arg(shell));
	if (ft_strlen(arg->line) == 0)
		return (0);
	if (arg->line[0] == '/' && ft_strlen(arg->line) > 1 && arg->line[1] != '/')
	{
		printing("cd: ", arg->line, ": No such file or directory\n", 2);
		//free_and_exit();
		return (1);
	}
	if (ft_strncmp(arg->line, "~", 1) != 0 && is_file(arg->line) == 0)
	{
		printing("cd: ", arg->line, ": Not a directory\n", 2);
		//free_and_exit();
		return (1);
	}
	return (2);
}

static int cd_no_arg(t_shell *shell)
{
	if (shell->home == NULL)
	{
		printing("cd", ": HOME not set\n", "", 2);
		//free_and_exit();
		return (1);
	}
	if (is_directory_new(shell->home) == 1)
	{
		printing("cd: ", shell->home, ": No such file or directory\n", 2);
		//free_and_exit();
		return (1);
	}
	if (chdir(shell->home) == -1)
	{
		perror("chdir() error");
		//free_and_exit();
	}
	else
		update_pwd(&shell->envp_copy);
	return (0);
}

static char	*get_new_path(t_shell *shell, t_token *arg)
{
	char	*old_path;
	char	*new_path;
	int		len;
	
	if (ft_strncmp(arg->line, "//\0", 3) == 0)
		return(ft_strdup("//"));
	if (ft_strncmp(arg->line, "/", 1) == 0)
		return(ft_strdup("/"));
	if (ft_strncmp(arg->line, "~", 1) == 0)
	{
		if (shell->home == NULL)
		{
			printing("cd", ": HOME not set\n", "", 2);
			//free_and_exit();
			return (NULL);
		}
		len = ft_strlen(shell->home) + ft_strlen(arg->line + 1) + 2;
		new_path = (char *)malloc(len * sizeof(char));
		if (new_path == NULL)
		{
			perror("malloc error");
			//free_and_exit();
		}
		new_path = ft_strjoin(shell->home, arg->line + 1);
		if (new_path == NULL)
		{
			perror("malloc error");
			//free_and_exit();
		}
	}
	else
	{
		old_path = (char *)malloc(BUFF_SIZE * sizeof(char));
		if (old_path == NULL)
		{
			perror("malloc error");
			//free_and_exit();
		}
		if (getcwd(old_path, BUFF_SIZE) == NULL)
		{
			perror("getcwd error");
			//free_and_exit();
		}
		len = ft_strlen(old_path) + ft_strlen(arg->line) + 2;
		new_path = (char *)malloc(len * sizeof(char));
		if (new_path == NULL)
		{
			perror("malloc error");
			//free_and_exit();
		}
		new_path = ft_strjoin_four(old_path, "/", arg->line, "");
		if (new_path == NULL)
		{
			perror("malloc error");
			//free_and_exit();
		}
		free(old_path);
	}
	return(new_path);
}

static int	access_new_path(t_shell *shell, t_token *arg, char *new_path)
{
	if (access(new_path, F_OK) == -1 && errno == ENOENT)
	{
		if (arg->line[0] == '~')
			printing("cd: ", new_path, ": No such file or directory\n", 2);
		else
			printing("cd: ", arg->line, ": No such file or directory\n", 2);
		free(new_path);
		//free_and_exit();
		return (1);
	}
	//set pwd in env
	if (chdir(new_path) == -1)
	{
    	perror("chdir() error");
		free(new_path);
		//free_and_exit();
	}
	else
	{
		update_pwd(&shell->envp_copy);
		free(new_path);
		//free_and_exit();
	}
	return (0);
}