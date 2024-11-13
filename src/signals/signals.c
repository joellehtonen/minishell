/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklimchu <aklimchu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 13:32:31 by jlehtone          #+#    #+#             */
/*   Updated: 2024/11/13 16:02:25 by aklimchu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

sig_atomic_t	g_signal = 0;

void	clear_input_normal(int signal);
void	clear_input_subprocess(int signal);
void	child_signals(int sig);

void	set_up_signals(t_shell *shell)
{
	struct sigaction	protocol;

	sigemptyset(&protocol.sa_mask);
	protocol.sa_flags = SA_SIGINFO | SA_RESTART;
	if (shell->in_here_doc == true || shell->in_child == true)
		protocol.sa_handler = &clear_input_subprocess;
	else
		protocol.sa_handler = &clear_input_normal;
		
	if (sigaction(SIGINT, &protocol, NULL) < 0)
		error_printer(shell, "", SIGNAL_ERROR, true);
	protocol.sa_handler = SIG_IGN;
	if (sigaction(SIGQUIT, &protocol, NULL) < 0)
		error_printer(shell, "", SIGNAL_ERROR, true);
	// if (shell->in_child == true)
	// {
	// 	// signal(SIGINT, SIG_DFL);
	// 	// signal(SIGQUIT, SIG_DFL);
	// 	signal(SIGINT, child_signals);
	// 	signal(SIGQUIT, child_signals);
	// }
	// else if (shell->in_here_doc == true)
	// {
	// 	signal(SIGINT, clear_input_subprocess);
	// 	signal(SIGQUIT, SIG_IGN);
	// }
	// else 
	// {
	// 	signal(SIGINT, clear_input_normal);
	// 	signal(SIGQUIT, SIG_IGN);
	// }
}

void	clear_input_normal(int signal)
{
	g_signal = signal;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	clear_input_subprocess(int signal)
{
	g_signal = signal;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
}

void	child_signals(int sig)
{
	if (sig == SIGINT)
	{
		printf("\n");
	}
	if (sig == SIGQUIT)
	{
		printf("Quit\n");
		exit(0);
	}
}
