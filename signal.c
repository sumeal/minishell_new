/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abin-moh <abin-moh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 13:59:07 by abin-moh          #+#    #+#             */
/*   Updated: 2025/04/21 15:41:18 by abin-moh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_signal_child_exit(int signum)
{
	(void)signum;
	write(STDOUT_FILENO, "Quit (core dumped)\n", 19);
}

void change_signal(int to_child)
{   
    if (to_child)
    {
		signal(SIGINT, handle_signal_child);
		signal(SIGQUIT, handle_signal_child_exit);

    }
    else
    {
		signal(SIGINT, handle_signal_parent);
		signal(SIGQUIT, SIG_IGN);
    }
}

void	handle_signal_parent(int signum)
{
	if (signum == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
		g_signal = 130;
	}
}

void	setup_signal_handlers(struct termios *original_term,
			struct termios *new_term)
{
	tcgetattr(STDIN_FILENO, original_term);
	*new_term = *original_term;
	new_term->c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, new_term);
	change_signal(0);
}

void	handle_signal_child(int signum)
{
	(void)signum;
	write(STDOUT_FILENO, "\n", 1);
}

void	setup_signal_heredoc(void)
{
	struct termios original;
	
	tcgetattr(STDIN_FILENO, &original);
	original.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &original);
	signal(SIGINT, SIG_IGN);
	// signal(SIGINT, handle_signal_heredoc);
	signal(SIGQUIT, SIG_IGN);
}
