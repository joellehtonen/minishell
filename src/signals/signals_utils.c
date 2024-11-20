/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlehtone <jlehtone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 16:07:15 by jlehtone          #+#    #+#             */
/*   Updated: 2024/11/20 10:11:49 by jlehtone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// sigint behavior normally
void	clear_input_normal(int signal)
{
	g_signal = signal;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

// sigint behavior in here_doc or child process
// behaves differently if signal came from SIGQUIT handler
void	clear_input_subprocess(int signal)
{
	if (g_signal == SIGQUIT)
	{
		return ;
	}
	else
	{
		g_signal = signal;
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
	}
}

// sigquit behavior in child process
// sends SIGINT to reset the terminal
void	quit_process(int signal)
{
	g_signal = signal;
	write(2, "Quit\n", 5);
	kill(0, SIGINT);
}
