#include "../../inc/minishell.h"

void	assign_type(t_token **token)
{
	t_token	*temp;
	t_token	*previous;
	
	temp = *token;
	previous = temp;
	while (temp)
	{
		if (ft_strncmp(temp->line, "|\0", 2) == 0)
			temp->type = PIPE1;
		else if (ft_strncmp(temp->line, "<\0", 2) == 0 || ft_strncmp(temp->line, ">\0", 2) == 0)
			temp->type = REDIR1;
		else if (ft_strncmp(temp->line, "<<\0", 3) == 0 || ft_strncmp(temp->line, ">>\0", 3) == 0)
			temp->type = REDIR1;
		else if (temp->next && ft_strncmp(temp->next->line, "<\0", 2) == 0)
			temp->type = INPUT;
		else if (ft_strncmp(previous->line, ">\0", 2) == 0 || ft_strncmp(previous->line, ">>\0", 3) == 0)
			temp->type = OUTPUT;
		else if (ft_strncmp(previous->line, "<<\0", 3) == 0)
			temp->type = DELIM;
		else
			temp->type = ARG;	// how do we distinguish ARG from COMM?
		previous = temp;
		temp = temp->next;
	}
}