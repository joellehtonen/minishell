//42 header

#include "../inc/minishell.h"

static int	count_str(t_envp *envp_copy);

char	**envp_to_arr(t_envp *envp_copy)
{
	char	**envp_arr;
	t_envp	*temp;
	int		i;

	envp_arr = (char **)malloc((count_str(envp_copy) + 1) * sizeof(char *));
	if (envp_arr == NULL)
		return ((void *) 0);
	i = 0;
	temp = envp_copy;
	while (temp)
	{
		envp_arr[i] = ft_strdup(temp->line);
		if (envp_arr[i] == NULL)
			return (NULL/* free_m(envp_arr, i) */);
		i++;
		temp = temp->next;
	}
	envp_arr[i] = ((void *) 0);
	return (envp_arr);
}

static int	count_str(t_envp *envp_copy)
{
	t_envp	*temp;
	int		count;

	count = 0;
	temp = envp_copy;
	while (temp)
	{
		count++;
		temp = temp->next;
	}
	return (count);
}