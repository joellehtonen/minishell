// 42 header 

#include "../../inc/minishell.h"

void	check_for_output_no_recur(t_shell *shell, t_token *token, int loop_count, int error_node)
{
	t_token	*temp;
	char	*outfile;
	
	if (error_node == -1)
		return ;
	temp = find_token_index(token, loop_count, REDIR_OUTPUT, error_node);
	if (!temp || !temp->next || temp->next->type != OUTPUT)
		return ;
	outfile = temp->next->line;
	if (outfile && outfile[0] == '\0')
	{
		//printing(outfile, "", ": No such file or directory\n", 2); // what if $HOME?
		close_pipes_child(loop_count, &shell->exec); // free pids?
		exit(1);
	}
	else
	{
		if (ft_strncmp(temp->line, ">\0", 2) == 0)
			shell->exec->out = open(outfile, O_WRONLY | O_TRUNC | O_CREAT, 0777);
		else
			shell->exec->out = open(outfile, O_WRONLY | O_APPEND | O_CREAT, 0777);
		if (shell->exec->out == -1)
		{
			//is_directory(outfile, *shell->exec, 0, NULL);
			/* if (access(outfile, W_OK) == -1 && errno == EACCES)
				printing(outfile, "", ": Permission denied\n", 2); */
			close_pipes_child(loop_count, &shell->exec); // free pids?
			exit(1);
		}
	}
}