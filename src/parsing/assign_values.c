//42 HEADER

#include "../../inc/minishell.h"

static int	parse_here_doc(t_token *token)
{
	t_token	*temp;
	int		count;

	count = 0;
	//temp = find_token(token, loop_count, REDIR_INPUT);
	temp = token;
	while (temp)
	{
		if (temp->type == REDIR_INPUT && ft_strncmp(temp->line, "<<\0", 3) == 0 && \
			temp->next && temp->next->type == DELIM)
			count++;
		temp = temp->next;
	}
	return (count);
}

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
		else if (ft_strncmp(temp->line, "<\0", 2) == 0 || ft_strncmp(temp->line, "<<\0", 3) == 0)
			temp->type = REDIR_INPUT;
		else if (ft_strncmp(temp->line, ">\0", 2) == 0 || ft_strncmp(temp->line, ">>\0", 3) == 0)
			temp->type = REDIR_OUTPUT;
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

void	assign_level(t_token **token, t_exec **exec, t_shell *shell)
{
	int		level;
	t_token	*token_temp;
	t_exec	*exec_temp;
	
	level = 0;
	token_temp = *token;
	while (token_temp)
	{
		if (ft_strncmp(token_temp->line, "|\0", 2) == 0)
			level++;
		token_temp->level = level;
		token_temp = token_temp->next;
	}
	*exec = (t_exec *)malloc(sizeof(t_exec));
	if (*exec == NULL)
		error_printer(shell, MALLOC_FAIL, true);
	exec_temp = *exec;
	exec_temp->pipe_num = level;
	exec_temp->pipe_flag = 0;
	exec_temp->pipe = NULL;
	exec_temp->here_doc_num = parse_here_doc(*token);
}
