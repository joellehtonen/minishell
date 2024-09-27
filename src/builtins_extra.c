//42 header

#include "../inc/minishell.h"

static void envp_remove_if_line(t_envp **lst, char *data, int (*cmp)());

static int	check_len(char *line, char* data);

int	env_exec(t_envp *envp_copy)
{
	while (envp_copy)
	{
		printf("%s\n", envp_copy->line);
		envp_copy = envp_copy->next;
	}
	return (0);
}

int	unset_exec(t_envp **envp_copy, char *input)
{
	input = input + 6;
	envp_remove_if_line(envp_copy, input, ft_strncmp);
	return (0);
}

int	exit_exec(t_shell *shell)
{
	(void)shell;
	//free_and_exit();
	exit(0);
}

static void envp_remove_if_line(t_envp **lst, char *data, int (*cmp)())
{
	t_envp	*temp;
	int		len;
	
	if (lst == NULL || *lst == NULL)
		return;
	temp = *lst;
	len = check_len(temp->line, data);
	if (cmp(temp->line, data, len) == 0)
	{
		*lst = temp->next;
		free(temp);
		envp_remove_if_line(lst, data, cmp);
	}
	else
	{
		temp = *lst;
		envp_remove_if_line(&temp->next, data, cmp);
	}
}

static int	check_len(char *line, char* data)
{
	int	len;
	
	if (ft_strlen(line) - ft_strlen(ft_strchr(line, '=')) > \
		ft_strlen(data))
		len = ft_strlen(line) - ft_strlen(ft_strchr(line, '='));
	else
		len = ft_strlen(data);
	return (len);
}