/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlehtone <jlehtone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 11:31:56 by aklimchu          #+#    #+#             */
/*   Updated: 2024/11/22 13:01:49 by jlehtone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	get_here_doc(t_exec **exec, t_token *redir, int i, t_shell *shell);
static char	*get_here_doc_input(char *delim, t_shell *shell, int quotes);
static void	new_input(char **input, char **new_line, t_shell *shell, int quotes);
static char	*add_here_doc_memory(char *str, size_t add_len);

// The function implements here_doc functionality
int	here_doc(t_shell *shell)
{
	int		i;
	t_token	*redir;

	redir = shell->token_pointer;
	i = 0;
	while (i < shell->exec->here_doc_num)
	{
		redir = find_here_doc_token(redir);
		if (redir == NULL)
			return (1);
		if (get_here_doc(&shell->exec, redir, i, shell) == 130)
			return (130);
		redir = redir->next;
		i++;
	}
	return (0);
}

// The function saves data from here_doc to the write end of here_doc pipe
static int	get_here_doc(t_exec **exec, t_token *redir, int i, t_shell *shell)
{
	char	*delim;
	char	*here_doc_input;
	int		quotes;

	shell->in_here_doc = true;
	set_up_signals(shell);
	delim = redir->next->line;
	quotes = false;
	if (redir->next->quotes == true)
		quotes = true;
	here_doc_input = get_here_doc_input(delim, shell, quotes);
	if (!here_doc_input)
		return (130);
	if (pipe((*exec)->here_doc_pipe[i]) == -1)
	{
		free_str(&here_doc_input);
		error_printer(shell, "", PIPE_FAIL, true);
	}
	ft_putstr_fd(here_doc_input, (*exec)->here_doc_pipe[i][1]);
	free_str(&here_doc_input);
	close((*exec)->here_doc_pipe[i][1]);
	return (0);
}

// The function reads here_doc input line by line
static char	*get_here_doc_input(char *delim, t_shell *shell, int quotes)
{
	char	*new_line;
	char	*here_doc_input;
	int		null_return;

	new_line = NULL;
	null_return = 0;
	here_doc_input = ft_strdup("");
	if (here_doc_input == NULL)
		error_printer(shell, "", MALLOC_FAIL, true);
	ft_printf("> ");
	new_line = get_next_line(0);
	if (new_line == NULL)
		null_return = null_signal(shell, delim);
	while (new_line)
	{
		if (!ft_strncmp(new_line, delim, ft_strlen(delim))
			&& ft_strchr_index(new_line, '\n') == ft_strlen(delim))
			break ;
		new_input(&here_doc_input, &new_line, shell, quotes);
		if (new_line == NULL)
			null_return = null_signal(shell, delim);
	}
	if (free_here_doc(new_line, here_doc_input, null_return, shell) == 1)
		return (NULL);
	return (here_doc_input);
}

// The function saves input from new line to the string
// containing input from previous lines
static void	new_input(char **input, char **new_line, t_shell *shell, int quotes)
{
	if (quotes == false)
		expand_here_doc(shell, &(*new_line));
	*input = add_here_doc_memory(*input, ft_strlen(*new_line));
	if (*input == NULL)
	{
		free_str(new_line);
		error_printer(shell, "", MALLOC_FAIL, true);
	}
	*input = ft_strncat(*input, *new_line, \
			ft_strlen(*input) + ft_strlen(*new_line) + 1);
	free_str(new_line);
	ft_printf("> ");
	*new_line = get_next_line(0);
}

// The function allocates additional memory to the string
// which is used to save here_doc input
static char	*add_here_doc_memory(char *str, size_t add_len)
{
	char	*temp;
	size_t	old_len;

	if (str == NULL)
		str = ft_strdup("");
	if (str == NULL)
		return (NULL);
	old_len = ft_strlen(str);
	temp = (char *)malloc(old_len + add_len + 1);
	if (temp == NULL)
	{
		free_str(&str);
		return (NULL);
	}
	while (*str)
	{
		*temp = *str;
		temp++;
		str++;
	}
	*temp = '\0';
	free(str - old_len);
	return (temp - old_len);
}
