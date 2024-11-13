/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklimchu <aklimchu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 11:31:56 by aklimchu          #+#    #+#             */
/*   Updated: 2024/11/13 16:01:25 by aklimchu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	get_here_doc(t_exec **exec, t_token *redir, int i, t_shell *shell);
static char	*get_here_doc_input(char *delim, t_shell *shell);
static void	new_input(char **input, char **new_line, t_shell *shell);
static char	*add_here_doc_memory(char *str, size_t add_len);

// The function implements here_doc scenario
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
		get_here_doc(&shell->exec, redir, i, shell);
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

	shell->in_here_doc = true;
	set_up_signals(shell);
	delim = redir->next->line;
	here_doc_input = get_here_doc_input(delim, shell);
	if (!here_doc_input)
		return (1);
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
static char	*get_here_doc_input(char *delim, t_shell *shell)
{
	char	*new_line;
	char	*new_line2;
	char	*here_doc_input;

	new_line = NULL;
	here_doc_input = ft_strdup("");
	if (here_doc_input == NULL)
		error_printer(shell, "", MALLOC_FAIL, true);
	new_line = readline("> ");
	if (new_line == NULL)
		null_signal(shell, delim);
	if (new_line)
	{
		new_line2 = ft_strjoin(new_line, "\n");
		// malloc error
		free(new_line);
	}
	else
		new_line2 = new_line;
	while (new_line2)
	{
		if (!ft_strncmp(new_line2, delim, ft_strlen(delim)) && \
			ft_strchr_fix(new_line2, '\n') == ft_strlen(delim))
			break ;
		new_input(&here_doc_input, &new_line2, shell);
		if (new_line2 == NULL)
			null_signal(shell, delim);
	}
	free_str(&new_line2);
	shell->in_here_doc = false;
	set_up_signals(shell);
	return (here_doc_input);
}

// The function saves input from new line to the string
// containing input from previous lines
static void	new_input(char **input, char **new_line, t_shell *shell)
{
	char	*temp;

	*input = add_here_doc_memory(*input, ft_strlen(*new_line));
	if (*input == NULL)
	{
		free_str(new_line);
		error_printer(shell, "", MALLOC_FAIL, true);
	}
	*input = ft_strncat(*input, *new_line, \
			ft_strlen(*input) + ft_strlen(*new_line) + 1);
	free_str(new_line);
	temp = readline("> ");
	if (temp)
	{
		*new_line = ft_strjoin(temp, "\n");
		// malloc error
		free(temp);
	}
	else
		*new_line = temp;
}

// The function allocated additional memory to the string
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
