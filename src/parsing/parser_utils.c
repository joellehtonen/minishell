// 42 HEADER

#include "../../inc/minishell.h"

int	is_valid_redir(t_shell *shell, int index1, int index2)
{
	char c1;
	char c2;
	
	c1 = shell->user_input[index1];
	c2 = shell->user_input[index2];
	if (isIO(shell, index2) == false)
		return (true);
	if ((c1 == '<' && c2 == '<')
		|| (c1 == '<' && c2 == '>')
		|| (c1 == '>' && c2 == '>'))
		return (true);
	else
		return (false);
}

int isspaces(char c)
{
	// char	c;

	// c = shell->user_input[index];
	if (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r')
		return (true);
	else
		return (false);
}

int isIO(t_shell *shell, int index)
{
	char	c;

	c = shell->user_input[index];
	if	(c == '|')
		return (PIPE);
	if (c == '<' || c == '>')
		return (REDIR);
	else
		return (false);
}

int isquote(t_shell *shell, int index)
{
	char	c;

	c = shell->user_input[index];
	// if (c == '\'')
	// 	return (S_QUOTE);
	// if (c == '\"')
	// 	return (D_QUOTE);
	if (c == '\'' || c == '\"')
		return (true);
	return (false);
}
