//42 header

#include "../inc/minishell.h"

void	free_shell(t_shell **shell, int free_envp)
{
	free_str(&(*shell)->user_input);
	free_str(&(*shell)->pwd);
	free_str(&(*shell)->prompt);
	free_str(&(*shell)->new_path);
	//free_exec(&(*shell)->exec);
	delete_all_tokens(&(*shell)->token_pointer);
	if (free_envp == true)
		(void)free_envp;
		//free_envp((*shell)->envp_copy);
}

void	free_double_arr(char ***arr)
{
	int		i;

	i = 0;
	if (*arr)
	{
		while ((*arr)[i])
			free_str(&(*arr)[i++]);
		//free(*arr);
		*arr = NULL;
	}
}

int	free_two_str(char **str1, char **str2)
{
	if (*str1)
	{
		free(*str1);
		*str1 = NULL;
	}
	if (*str2)
	{
		free(*str2);
		*str2 = NULL;
	}
	return (1);
}

int	free_str(char **str1)
{
	if (*str1)
	{
		free(*str1);
		*str1 = NULL;
	}
	return (1);
}