#include "../../inc/minishell.h"

void	assign_type(t_token **token)
{
	t_token	*temp;
	t_token	*previous;
	int		comm_flag;
	
	temp = *token;
	previous = temp;
	comm_flag = 0;
	while (temp)
	{
		if (ft_strncmp(temp->line, "|\0", 2) == 0)
		{
			temp->type = PIPE;
			comm_flag = 0;
		}
		else if (ft_strncmp(temp->line, "<\0", 2) == 0 || ft_strncmp(temp->line, ">\0", 2) == 0)
			temp->type = REDIR;
		else if (ft_strncmp(temp->line, "<<\0", 3) == 0 || ft_strncmp(temp->line, ">>\0", 3) == 0)
			temp->type = REDIR;
		else if (ft_strncmp(previous->line, "<\0", 2) == 0)
			temp->type = INPUT;
		else if (ft_strncmp(previous->line, ">\0", 2) == 0 || ft_strncmp(previous->line, ">>\0", 3) == 0)
			temp->type = OUTPUT;
		else if (ft_strncmp(previous->line, "<<\0", 3) == 0)
			temp->type = DELIM;
		else if (comm_flag == 0)
		{
			temp->type = COMM;
			comm_flag = 1;
		}
		else
			temp->type = ARG;
		previous = temp;
		temp = temp->next;
	}
}

void	assign_level(t_token **token, t_exec *exec)
{
	int		level;
	t_token	*temp;
	
	level = 0;
	temp = *token;
	while (temp)
	{
		if (ft_strncmp(temp->line, "|\0", 2) == 0)
			level++;
		temp->level = level;
		temp = temp->next;
	}
	exec->pipe_num = level;
}