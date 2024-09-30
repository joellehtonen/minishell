/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlehtone <jlehtone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 13:32:31 by jlehtone          #+#    #+#             */
/*   Updated: 2024/09/30 16:32:28 by jlehtone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static void	clear_input(int signal)
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
	if (sigaction(SIGINT, &protocol, NULL) < 0)
		error_printer(shell, SIGNAL_ERROR, true);
}
