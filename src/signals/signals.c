/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlehtone <jlehtone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 13:32:31 by jlehtone          #+#    #+#             */
/*   Updated: 2024/11/14 10:41:45 by jlehtone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

sig_atomic_t	g_signal = 0;

static void	setup_sigint(t_shell *shell, struct sigaction *sigint);
static void	setup_sigquit(t_shell *shell, struct sigaction *sigquit);

void	set_up_signals(t_shell *shell)
{
	struct sigaction	sigint;
	struct sigaction	sigquit;

	setup_sigint(shell, &sigint);
	if (sigaction(SIGINT, &sigint, NULL) < 0)
		error_printer(shell, "", SIGNAL_ERROR, true);
	setup_sigquit(shell, &sigquit);
	if (sigaction(SIGQUIT, &sigquit, NULL) < 0)
		error_printer(shell, "", SIGNAL_ERROR, true);
	return ;
}

static void	setup_sigint(t_shell *shell, struct sigaction *sigint)
{
	sigemptyset(&sigint->sa_mask);
	sigint->sa_flags = SA_SIGINFO;
	if (shell->in_here_doc == true || shell->in_child == true)
	{
		if (shell->in_child == true)
			sigint->sa_flags = SA_RESTART;
		sigint->sa_handler = &clear_input_subprocess;	
	}
	else
		sigint->sa_handler = &clear_input_normal;
	return ;
}

static void	setup_sigquit(t_shell *shell, struct sigaction *sigquit)
{
	sigemptyset(&sigquit->sa_mask);
	sigquit->sa_flags = SA_SIGINFO;
	if (shell->in_child == true)
		sigquit->sa_handler = &quit_process;
	else
		sigquit->sa_handler = SIG_IGN;
	return ;
}
