/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_input_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlehtone <jlehtone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 08:50:15 by aklimchu          #+#    #+#             */
/*   Updated: 2024/11/18 15:43:00 by jlehtone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// defaults the global variable where signals are saved
// and alters the exit code in case of ctrl+C (sigint)
void	handle_readline_signal(t_shell *shell)
{
	if (g_signal == SIGINT || g_signal == SIGQUIT)
	{
		if (g_signal == SIGINT)
			shell->exit_code = 130;
		g_signal = 0;
	}
	return ;
}

// defines how minishell behaves when it receives EOF from ctrl+D
void	null_signal(t_shell *shell, char *arg)
{
	if (shell->in_child == false && shell->in_here_doc == false)
	{
		shell->exit_code = 0;
		printf("exit\n");
		free_and_exit(shell, 0);
	}
	if (shell->in_here_doc == true && g_signal != SIGINT)
	{
		printf("Warning: here-document delimited by end-of-file ");
		printf("(wanted '%s')\n", arg);
	}
	return ;
}

// The function assigns values before printing the minishell prompt
int	fill_values_before_prompt(t_shell **shell)
{
	char	*new_pwd;	

	new_pwd = get_pwd((*shell)->home, *shell);
	if (new_pwd)
	{
		free_str(&(*shell)->pwd);
		(*shell)->pwd = new_pwd;
	}
	(*shell)->envp_str = NULL;
	(*shell)->exec = NULL;
	(*shell)->only_one_builtin = 0;
	if (copy_path(&(*shell)->path, (*shell)->envp_copy) == 1)
		return (1);
	if (copy_home(&(*shell)->home, (*shell)->envp_copy) == 1)
		return (1);
	return (copy_uname(&(*shell)->uname, (*shell)->envp_copy));
}

// The function is used to join four strings into one
char	*ft_strjoin_four(char *s1, char *s2, char *s3, char *s4)
{
	char	*new_s;
	size_t	totallen;

	totallen = ft_strlen(s1) + ft_strlen(s2) + ft_strlen(s3) + ft_strlen(s4);
	new_s = (char *)malloc((totallen + 1) * sizeof(char));
	if (new_s == NULL)
		return ((void *)0);
	while (*s1)
		*(new_s++) = *(s1++);
	while (*s2)
		*(new_s++) = *(s2++);
	while (*s3)
		*(new_s++) = *(s3++);
	while (*s4)
		*(new_s++) = *(s4++);
	*new_s = '\0';
	return (new_s - totallen);
}
