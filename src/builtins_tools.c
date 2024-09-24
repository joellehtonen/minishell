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
	pwd_move = 0;
	if (ft_strncmp(pwd, home, 1024) == 0)
		return(ft_strdup("~")); // protect
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
