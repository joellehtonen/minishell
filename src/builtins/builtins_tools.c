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

int	too_many_arg_cd(char *input)
{
	int		s_quote_flag;
	int		d_quote_flag;

	s_quote_flag = 0;
	d_quote_flag = 0;
	input = input + 3;
	while (*input == ' ')
		input++;
	while(*input)
	{
		if (*input == '\'')
			s_quote_flag++;
		if (*input == '\"')
			d_quote_flag++;
		if (*input == ' ' && s_quote_flag % 2 == 0 &&\
			d_quote_flag % 2 == 0 && only_spaces(input) == 1)
			return (1);
		input++;
	}
	return (0);
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
		return (0);
	else
		return (1);
}

int	update_pwd(t_envp **envp_copy)
{
	t_envp	*temp;
	t_envp	*new;
	t_envp	*new2;
	char	*pwd;
	char	*export_new;

	temp = *envp_copy;
	while (*envp_copy)
	{
		if (ft_strncmp((*envp_copy)->line, "PWD=", 4) == 0)
		{
			envp_remove_if_export(envp_copy, (*envp_copy)->line, ft_strncmp);
			new = ft_lstnew_envp((*envp_copy)->line);
			if (new == NULL)
			{
				//free_and_exit(...);
				return(0);
			}
			ft_lstadd_back_envp(envp_copy, new);
		}
		*envp_copy = (*envp_copy)->next;
	}
	*envp_copy = temp;
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
	envp_remove_if_export(envp_copy, export_new, ft_strncmp);
	new2 = ft_lstnew_envp(export_new);
	if (new2 == NULL)
		{
			//free_and_exit(...);
			return(0);
		}
		ft_lstadd_back_envp(envp_copy, new2);
	return (0);
}