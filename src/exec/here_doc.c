/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlehtone <jlehtone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 11:31:56 by aklimchu          #+#    #+#             */
/*   Updated: 2024/11/04 13:56:15 by jlehtone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int get_here_doc(t_exec **exec, t_token *redir, int i, t_shell *shell);

static char	*get_here_doc_input(char *delim, t_shell *shell);

static void	new_here_doc_input(char **hd_input, char **new_line, t_shell *shell);

static char	*add_here_doc_memory(char *str, size_t add_len);

int here_doc(t_shell *shell)
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

static int get_here_doc(t_exec **exec, t_token *redir, int i, t_shell *shell)
{
	char	*delim;
	char	*here_doc_input;

	delim = redir->next->line;
	here_doc_input = get_here_doc_input(delim, shell);
	if (!here_doc_input)
		return (1);
	if (pipe((*exec)->here_doc_pipe[i]) == -1)
	{
		free_str(&here_doc_input);
   		error_printer(shell, SYNTAX_ERROR, true);
	}
	ft_putstr_fd(here_doc_input, (*exec)->here_doc_pipe[i][1]);
	free_str(&here_doc_input);
	close((*exec)->here_doc_pipe[i][1]);
	return (0);
}

//The function reads the user input with the help of get_next_line tool
static char	*get_here_doc_input(char *delim, t_shell *shell)
{
	char	*new_line;
	char	*hd_input;

	new_line = NULL;
	hd_input = ft_strdup("");
	if (hd_input == NULL)
	   	error_printer(shell, MALLOC_FAIL, true);
	ft_printf("> ");
	new_line = get_next_line(0);
	while (new_line)
	{
		if (!ft_strncmp(new_line, delim, ft_strlen(delim)) && \
			ft_strchr_fix(new_line, '\n') == ft_strlen(delim))
			break ;
		new_here_doc_input(&hd_input, &new_line, shell);
	}
	free_str(&new_line);
	return (hd_input);
}

//The function concatenates the string read from the new line
//to the input from previous lines
static void	new_here_doc_input(char **hd_input, char **new_line, t_shell *shell)
{
	*hd_input = add_here_doc_memory(*hd_input, ft_strlen(*new_line));
	if (*hd_input == NULL)
	{
		free_str(new_line); // free here_doc_input?
		error_printer(shell, MALLOC_FAIL, true);
	}
	*hd_input = ft_strncat(*hd_input, *new_line, \
			ft_strlen(*hd_input) + ft_strlen(*new_line) + 1);
	free_str(new_line);
	ft_printf("> ");
	*new_line = get_next_line(0);
}

//The function allocates new memory to store user input from command line
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
		return (NULL);
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