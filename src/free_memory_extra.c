//42  HEADER

#include "minishell.h"

static void	delete_one_envp_node(t_envp *node)
{
	if (!node || !node->line)
		return ;
	free(node->line);
	free(node);
}

void	delete_envp(t_envp **copy_envp)
{
	t_envp	*temp;

	if (!(*copy_envp))
		return ;
	while (*copy_envp)
	{
		temp = (*copy_envp)->next;
		delete_one_envp_node(*copy_envp);
		*copy_envp = temp;
	}
	*copy_envp = NULL;
}

void	delete_envp_part(t_envp **copy_envp, int count)
{
	t_envp	*temp;
	int		i;

	if (!(*copy_envp))
		return ;
	i = 0;
	while (*copy_envp && i <= count)
	{
		temp = (*copy_envp)->next;
		delete_one_envp_node(*copy_envp);
		*copy_envp = temp;
		i++;
	}
	*copy_envp = NULL;
}