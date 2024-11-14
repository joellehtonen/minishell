/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlehtone <jlehtone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 16:07:15 by jlehtone          #+#    #+#             */
/*   Updated: 2024/11/14 10:49:34 by jlehtone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	clear_input_normal(int signal)
{
	g_signal = signal;
	write(1, "\n", 1);
	//printf("signal is now %d\n", g_signal);
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

void	quit_process(int signal)
{
	g_signal = signal;
	write(2, "Quit\n", 5);
	rl_on_new_line();
	rl_replace_line("", 0);
}
