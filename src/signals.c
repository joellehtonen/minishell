/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlehtone <jlehtone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 13:32:31 by jlehtone          #+#    #+#             */
/*   Updated: 2024/10/02 11:17:39 by jlehtone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	clear_input(int signal)
{
	(void)signal;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	set_up_signals(t_shell *shell)
{
	struct sigaction	protocol;
	
	sigemptyset(&protocol.sa_mask);
	protocol.sa_handler = &clear_input;
	protocol.sa_flags = SA_SIGINFO | SA_RESTART;
	if (sigaction(SIGINT, &protocol, NULL) < 0)
		error_printer(shell, SIGNAL_ERROR, true);
	protocol.sa_handler = SIG_IGN;
	if (sigaction(SIGQUIT, &protocol, NULL) < 0)
		error_printer(shell, SIGNAL_ERROR, true);
}
