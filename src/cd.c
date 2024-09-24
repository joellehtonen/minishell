//42 header

#include "../inc/minishell.h"

static char	*no_white_spaces(char *str);

static int	count_w_sp(char *str);

int	cd_exec(t_shell *shell)
{
	char	*old_path;
	char	*new_path;
	int		len;
	
	if (ft_strlen(shell->user_input) == 2 ||\
	(ft_strlen(shell->user_input) == 3 && ft_strncmp(shell->user_input, "cd ", 3) == 0))
	{
		if (chdir(shell->home) == -1)
		{
    		perror("chdir() error");
			//free_and_exit();
		}
		return (0);
	}
	if (too_many_arg_cd(shell->user_input) == 1)
	{
		printing("cd", ": too many arguments\n", 2);
		//free_and_exit();
		//exit(1); // can add when in child process
		return (1);
	}
	// ------ filtering old path provided by user
	if (ft_strncmp(shell->user_input, "cd ~", 4) == 0)
	{
		shell->user_input = no_white_spaces(shell->user_input);
		len = ft_strlen(shell->home) + ft_strlen(shell->user_input + 1) + 2;
		new_path = (char *)malloc(len * sizeof(char));
		if (new_path == NULL)
		{
			perror("malloc error");
			//free_and_exit();
		}
		new_path = ft_strjoin(shell->home,shell->user_input + 1);
		if (new_path == NULL)
		{
			perror("malloc error");
			//free_and_exit();
		}
		printf("new path - %s\n", new_path);
	}
	else
	{
			//old path
		old_path = (char *)malloc(B_SIZE * sizeof(char));
		if (old_path == NULL)
		{
			perror("malloc error");
			//free_and_exit();
		}
		if (getcwd(old_path, 1024) == NULL)
		{
			perror("getcwd error");
			//free_and_exit();
		}
		printf("old path - %s\n", old_path);
		shell->user_input = no_white_spaces(shell->user_input);
		len = ft_strlen(old_path) + ft_strlen(shell->user_input) + 2;
		new_path = (char *)malloc(len * sizeof(char));
		if (new_path == NULL)
		{
			perror("malloc error");
			//free_and_exit();
		}
		new_path = ft_strjoin_four(old_path, "/",shell->user_input, "");
		if (new_path == NULL)
		{
			perror("malloc error");
			//free_and_exit();
		}
		free(old_path);
		printf("new path - %s\n", new_path);
	}
	if (access(new_path, F_OK) == -1 && errno == ENOENT)
	{
		printing(shell->user_input, ": No such file or directory\n", 2);
		free(new_path);
		//free_and_exit();
		//exit(127); // can add when in child process
		return (1);
	}
	if (chdir(new_path) == -1)
	{
    	perror("chdir() error");
		//free_and_exit();
	}
	free(new_path);
	return (0);
}

static char	*no_white_spaces(char *str)
{
	char	*new_str;
	int		new_len;

	str = str + 3;
	new_len = ft_strlen(str) - count_w_sp(str);
	new_str = (char *)malloc((new_len + 1) * sizeof(char));
	while (*str == ' ')
		str++;
	while(*str)
	{
		if ((*str == ' ' && only_spaces(str) == 0) ||\
		*str == '\\' || *str == '\"' || *str == '\'')
			str++;
		else
		{
			*new_str = *str;
			str++;
			new_str++;
		}
	}
	*new_str = '\0';
	new_str = new_str - new_len;
	return (new_str);
}

static int	count_w_sp(char *str)
{
	int	count;

	count = 0;
	while (*str == ' ')
	{
		count++;
		str++;
	}
	while(*str)
	{
		if (*str == ' ' && only_spaces(str) == 0)
			count++;
		if (*str == '\\' || *str == '\"' || *str == '\'')
			count++;
		str++;
	}
	return (count);
}
