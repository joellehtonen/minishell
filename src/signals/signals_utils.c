/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlehtone <jlehtone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 16:07:15 by jlehtone          #+#    #+#             */
/*   Updated: 2024/11/14 09:59:59 by jlehtone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

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

// void	reset_terminal_state(void)
// {
// 	struct termios term;
	
// 	tcgetattr(STDIN_FILENO, &term);
// 	term.c_iflag = ICRNL | IXON | BRKINT;
// 	term.c_oflag = OPOST | ONLCR;
// 	term.c_cflag = CS8 | CREAD | CLOCAL | HUPCL;
// 	term.c_lflag = ICANON | ECHO | ECHOE | ECHOK | ISIG | IEXTEN;
// 	term.c_cc[VINTR] = 3;
// 	term.c_cc[VQUIT] = 28;
// 	term.c_cc[VEOF] = 4;
// 	tcsetattr(STDIN_FILENO, TCSANOW, &term);
// }

void	quit_process(int signal)
{
	g_signal = signal;
	write(2, "Quit\n", 5);
	rl_on_new_line();
	rl_replace_line("", 0);
	//rl_redisplay();
	//reset_terminal_state();
	
}

