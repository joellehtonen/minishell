//42 header

#include "../../inc/minishell.h"

void	ft_lstadd_back_envp(t_envp **lst, t_envp *new)
{
	t_envp	*temp;

	if ((!lst) || (!new))
		return ;
	if (*lst == NULL)
		*lst = new;
	else
	{
		temp = ft_lstlast_envp(*lst);
		temp->next = new;
	}
	return ;
}

t_envp	*ft_lstnew_envp(char *content)
{
	t_envp	*new;

	new = (t_envp *)malloc(sizeof(*new));
	if (new == NULL)
		return ((void *) 0);
	new->line = ft_strdup(content);
	if (new->line == NULL)
		return (NULL);
	new->next = NULL;
	return (new);
}

t_envp	*ft_lstnew_envp_no_strdup(char *content)
{
	t_envp	*new;

	new = (t_envp *)malloc(sizeof(*new));
	if (new == NULL)
		return ((void *) 0);
	new->line = content;
	if (new->line == NULL)
		return (NULL);
	new->next = NULL;
	return (new);
}

t_envp	*ft_lstlast_envp(t_envp *lst)
{
	t_envp	*temp;

	if (lst == NULL)
		return ((void *) 0);
	temp = lst;
	while (temp->next != NULL)
	{
		temp = temp->next;
	}
	return (temp);
}
