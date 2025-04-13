/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muzz <muzz@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 08:32:49 by abin-moh          #+#    #+#             */
/*   Updated: 2025/04/08 10:56:41 by muzz             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_signal_heredoc(int signum)
{
	if (signum == SIGINT)
	{
		printf("\n^C");
		g_signal = 130;
	}
}

int	hd_printf(char *hd_delimeter)
{
	int		pipe_fd[2];
	char	*line;

	if (pipe(pipe_fd) < 0)
		return (-1);
	signal(SIGINT, handle_signal_heredoc);
	while (1)
	{
		line = readline("> ");
		if (!line || g_signal == 130)
		{
			close(pipe_fd[0]);
			close(pipe_fd[1]);
			return (-1);
		}
		if (ft_strcmp(line, hd_delimeter) == 0)
		{
			free(line);
			break ;
		}
		ft_putendl_fd(line, pipe_fd[1]);
		free(line);
	}
	close(pipe_fd[1]);
	return (pipe_fd[0]);
}

int	exit_function(t_cmd *commands, char **mini_envp, int *g_exit_status)
{
	if (!(commands->argv[1]))
	{
		printf("exit\n");
		exit_program(commands, mini_envp, g_exit_status);
	}
	else
		return (check_exit_value(commands, mini_envp, g_exit_status));
	return (1);
}
