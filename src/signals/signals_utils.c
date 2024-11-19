/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlehtone <jlehtone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 16:07:15 by jlehtone          #+#    #+#             */
/*   Updated: 2024/11/19 17:00:56 by jlehtone         ###   ########.fr       */
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
void	clear_input_subprocess(int signal)
{
	g_signal = signal;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
}

// resets terminal after sigquit signal
void	reset_terminal(void)
{
	struct termios term;
	
	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag = ICRNL | IXON | BRKINT;
	term.c_oflag = OPOST | ONLCR;
	term.c_cflag = CS8 | CREAD | CLOCAL | HUPCL;
	term.c_lflag = ICANON | ECHO | ECHOE | ECHOK | ISIG | IEXTEN;
	// term.c_cc[VINTR] = 3;
	// term.c_cc[VQUIT] = 28;
	// term.c_cc[VEOF] = 4;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

// sigquit behavior in child process
void	quit_process(int signal)
{
	g_signal = signal;
	write(2, "Quit\n", 5);
	rl_on_new_line();
	rl_replace_line("", 0);
	//reset_terminal();
}
