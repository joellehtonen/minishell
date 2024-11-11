/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlehtone <jlehtone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 13:32:31 by jlehtone          #+#    #+#             */
/*   Updated: 2024/11/11 15:42:03 by jlehtone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

sig_atomic_t	g_signal = 0;

void	clear_input_normal(int signal);
void	clear_input_here_doc(int signal);

void	set_up_signals(t_shell *shell)
{
	struct sigaction	protocol;

	//cleanup_here_doc(shell);
	sigemptyset(&protocol.sa_mask);
	protocol.sa_flags = SA_SIGINFO;
	if (shell->in_subprocess== true)
		protocol.sa_handler = &clear_input_here_doc;
	else
		protocol.sa_handler = &clear_input_normal;
	if (sigaction(SIGINT, &protocol, NULL) < 0)
		error_printer(shell, "", SIGNAL_ERROR, true);
	protocol.sa_handler = SIG_IGN;
	if (sigaction(SIGQUIT, &protocol, NULL) < 0)
		error_printer(shell, "", SIGNAL_ERROR, true);
}

void	clear_input_normal(int signal)
{
	g_signal = signal;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	//add closing the pipes and file descriptors
	//free_and exit?
}

void	clear_input_here_doc(int signal)
{
	g_signal = signal;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
}
