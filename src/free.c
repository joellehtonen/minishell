//42  HEADER

#include "minishell.h"

void	free_double_arr(char **arr)
{
	int		i;

	i = 0;
	{
		while (arr[i])
			free(arr[i++]);
		free(arr);
		arr = NULL;
	}
}

int	free_two_str(char *str1, char *str2)
{
	if (str1)
	{
		free(str1);
		str1 = NULL;
	}
	if (str2)
	{
		free(str2);
		str2 = NULL;
	}
	return (1);
}

int	free_str(char *str1)
{
	if (str1)
	{
		free(str1);
		str1 = NULL;
	}
	return (1);
}
