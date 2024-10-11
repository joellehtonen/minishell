//42 header

#include "../../inc/minishell.h"

char	*get_pwd(char *home)
{
	char	*pwd;
	char	*new_pwd;
	int		pwd_move;
	
	pwd = (char *)malloc(BUFF_SIZE * sizeof(char));
	if (pwd == NULL)
	{
		perror("malloc error");
		//free_and_exit();
	}
	if (getcwd(pwd, BUFF_SIZE) == NULL)
	{
		perror("getcwd error");
		//free_and_exit();
	}
	if (ft_strncmp(pwd, "/", 1) == 0 && ft_strlen(pwd) == 1)
	{
		free(pwd);
		new_pwd = ft_strdup("/");
		return (new_pwd);
	}
	if (ft_strncmp(pwd, "/home", 5) == 0 && ft_strlen(pwd) == 5)
	{
		free(pwd);
		new_pwd = ft_strdup("/home");
		return (new_pwd);
	}
	if (home == NULL)
	{
		return (pwd);
	}
	if (ft_strncmp(pwd, home, 1024) == 0)
	{
		free(pwd);
		new_pwd = ft_strdup("~");
		return (new_pwd);
	}
	pwd_move = 0;
	while ((*home) && (*pwd) && *pwd == *home)
	{
		pwd++;
		home++;
		pwd_move++;
	}
	if (*pwd)
	{
		pwd++;
		pwd_move++;
	}
	if (pwd_move <= 1)
		return (pwd - pwd_move);
	new_pwd = ft_strjoin("~/", pwd);
	if (new_pwd == NULL)
	{
		perror("malloc error");
		//free_and_exit();
	}
	free(pwd - pwd_move);
	return(new_pwd);
}

int	only_spaces(char *str)
{
	while (*str)
	{
		if (*str != ' ')
			return (1);
		str++;
	}
	return (0);
}

int	is_directory_new(char *path)
{
	int		fd;
	char	buffer;
	ssize_t	result;

	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (1);
	result = read(fd, &buffer, 1);
	if (result < 0 && errno == EISDIR)
	{
		close(fd);
		return (0);
	}
	else
	{
		close(fd);
		return (1);
	}
}

int is_file(char *path)
{
	int		fd;
	char	buffer;
	ssize_t	result;

	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (1);
	result = read(fd, &buffer, 1);
	if (result < 0 && errno == EISDIR)
		return (1);
	else
		return (0);
}

int	update_pwd(t_envp **envp_copy)
{
	t_envp	*new;
	char	*pwd;
	char	*export_new;

	envp_remove_if_line(envp_copy, "PWD=", ft_strncmp);
	pwd = (char *)malloc(BUFF_SIZE * sizeof(char));
	if (pwd == NULL)
	{
		perror("malloc error");
		//free_and_exit();
	}
	if (getcwd(pwd, BUFF_SIZE) == NULL)
	{
		perror("getcwd error");
		//free_and_exit();
	}
	export_new = ft_strjoin("PWD=", pwd);
	if (export_new == NULL)
	{
		perror("malloc error");
		//free_and_exit();
	}
	new = ft_lstnew_envp_no_strdup(export_new);
	if (new == NULL)
	{
		//free_and_exit(...);
		return(0);
	}
	ft_lstadd_back_envp(envp_copy, new);
	return (0);
}