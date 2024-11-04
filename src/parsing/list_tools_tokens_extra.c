//42 header

#include "../../inc/minishell.h"

int	count_nodes_type(t_token *start, int token_type, int loop_count)
{
	t_token	*temp;
	int		i;
	
	temp = start;
	i = 0;
	while (temp && temp->level != loop_count)
		temp = temp->next;
	while (temp && temp->level == loop_count)
	{
		if (temp->type == token_type)
			i++;
		temp = temp->next;
	}
	if (!temp || temp->level != loop_count)
		return (i);
	return(i);
}
