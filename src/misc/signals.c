/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklimchu <aklimchu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 13:32:31 by jlehtone          #+#    #+#             */
/*   Updated: 2024/11/08 14:40:23 by aklimchu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	clear_input(int signal)
{
	(void)signal;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	//add closing the pipes and file descriptors
	//free_and exit?
}

void	set_up_signals(t_shell *shell)
{
	struct sigaction	protocol;

	sigemptyset(&protocol.sa_mask);
	protocol.sa_handler = &clear_input;
	protocol.sa_flags = SA_SIGINFO;
	if (sigaction(SIGINT, &protocol, NULL) < 0)
		error_printer(shell, "", SIGNAL_ERROR, true);
	protocol.sa_handler = SIG_IGN;
	if (sigaction(SIGQUIT, &protocol, NULL) < 0)
		error_printer(shell, "", SIGNAL_ERROR, true);
}
