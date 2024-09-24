//42 header

#include "../inc/minishell.h"

char	*get_pwd(char *home)
{
	char	*pwd;
	char	*new_pwd;
	int		pwd_move;
	
	pwd = (char *)malloc(B_SIZE * sizeof(char));
	if (pwd == NULL)
	{
		perror("malloc error");
		//free_and_exit();
	}
	if (getcwd(pwd, 1024) == NULL)
	{
		perror("getcwd error");
		//free_and_exit();
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
	printf("pwd: %s\n", pwd);
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