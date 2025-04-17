/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abin-moh <abin-moh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 13:59:07 by abin-moh          #+#    #+#             */
/*   Updated: 2025/04/17 16:14:35 by abin-moh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_signal_parent(int signum)
{
	if (signum == SIGINT)
	{
		printf("\n^C\n");
		rl_on_new_line();
		rl_replace_line("", 0);
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
	signal(SIGINT, handle_signal_parent);
	signal(SIGQUIT, SIG_IGN);
}

void	handle_signal_child(int signum)
{
	if (signum == SIGINT)
		ft_putstr_fd("^C\n", STDOUT_FILENO);
	else if (signum == SIGQUIT)
		ft_putstr_fd("^\\Quit (core dumped)\n", STDOUT_FILENO);
}

void	setup_signal_child(void)
{
	signal(SIGINT, handle_signal_child);
	signal(SIGQUIT, handle_signal_child);
}

void	setup_signal_heredoc(void)
{
	struct termios original;
	
	tcgetattr(STDIN_FILENO, &original);
	original.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &original);
	signal(SIGINT, handle_signal_heredoc);
	signal(SIGQUIT, SIG_IGN);
}
