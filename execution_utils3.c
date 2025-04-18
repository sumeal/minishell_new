/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abin-moh <abin-moh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 08:32:49 by abin-moh          #+#    #+#             */
/*   Updated: 2025/04/18 16:55:55 by abin-moh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_signal_heredoc(int signum)
{
	if (signum == SIGINT)
	{
		exit(130);
	}
}

int is_quoted_delimiter(const char *delim)
{
    size_t len = strlen(delim);
    return (len >= 2 && ((delim[0] == '\'' && delim[len-1] == '\'') ||
                         (delim[0] == '"' && delim[len-1] == '"')));
}

int hd_printf(char *hd_delimiter, int *g_exit_status)
{
    int pipe_fd[2];
    char *line;
    pid_t pid;
    int status;
    struct termios original_parent_termios;
    int stdin_is_tty = isatty(STDIN_FILENO);
    void (*orig_sigint)(int) = SIG_DFL;
    void (*orig_sigquit)(int) = SIG_DFL;

	int quoted = is_quoted_delimiter(hd_delimiter);

    if (stdin_is_tty && tcgetattr(STDIN_FILENO, &original_parent_termios) == -1)
	{
        perror("minishell: tcgetattr (parent)");
        return (-1);
    }
    if (pipe(pipe_fd) < 0)
	{
        perror("minishell: pipe");
        return (-1);
    }
    orig_sigint = signal(SIGINT, SIG_IGN);
    orig_sigquit = signal(SIGQUIT, SIG_IGN);
    pid = fork();

    if (pid < 0)
    {
        perror("minishell: fork");
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        signal(SIGINT, orig_sigint);
        signal(SIGQUIT, orig_sigquit);
        if (stdin_is_tty)
             tcsetattr(STDIN_FILENO, TCSANOW, &original_parent_termios);
        return (-1);
    }

    if (pid == 0)
    {
		rl_catch_signals = 0;
		setup_signal_heredoc();
		struct sigaction check;
		sigaction(SIGQUIT, NULL, &check);
		if (check.sa_handler == SIG_IGN)
    		printf("SIGQUIT is ignored\n");
		else
    		printf("SIGQUIT is NOT ignored\n");
        close(pipe_fd[0]);
        while (1)
        {
            line = readline("> ");
            if (!line) { // EOF
                close(pipe_fd[1]);
                exit(EXIT_SUCCESS);
            }
            if (ft_strcmp(line, hd_delimiter) == 0)
			{
                free(line);
                close(pipe_fd[1]);
                exit(EXIT_SUCCESS);
            }
			if (!quoted)
			{
				char *expanded = expand_lexem(line, mini_envp, *g_exit_status);
				free(line);
				line = expanded;
			}
            ft_putendl_fd(line, pipe_fd[1]);
            free(line);
        }
        exit(EXIT_FAILURE);
    }
    else
    {
        close(pipe_fd[1]);
        waitpid(pid, &status, 0);
        signal(SIGINT, orig_sigint);
        signal(SIGQUIT, orig_sigquit);
        if (stdin_is_tty)
		{
             if (tcsetattr(STDIN_FILENO, TCSANOW, &original_parent_termios) == -1)
			 {
                 perror("minishell: tcsetattr (parent restore)");
             }
        }
        if (WIFEXITED(status))
            *g_exit_status = WEXITSTATUS(status);
        else if (WIFSIGNALED(status))
            *g_exit_status = 128 + WTERMSIG(status);
        else
            *g_exit_status = 127;
        if (*g_exit_status == 130) {
            close(pipe_fd[0]);
            return (-1);
        }
        if (*g_exit_status == EXIT_SUCCESS)
		{
             return (pipe_fd[0]);
        }
		else
		{
            close(pipe_fd[0]);
            return (-1);
        }
    }
    return (-1);
}


// int	hd_printf(char *hd_delimeter)
// {
// 	int		pipe_fd[2];
// 	char	*line;

// 	if (pipe(pipe_fd) < 0)
// 		return (-1);
// 	setup_signal_heredoc();
// 	while (1)
// 	{
// 		line = readline("> ");
// 		if (!line || g_signal == 130)
// 		{
// 			close(pipe_fd[0]);
// 			close(pipe_fd[1]);
// 			return (-1);
// 		}
// 		if (ft_strcmp(line, hd_delimeter) == 0)
// 		{
// 			free(line);
// 			break ;
// 		}
// 		ft_putendl_fd(line, pipe_fd[1]);
// 		free(line);
// 	}
// 	close(pipe_fd[1]);
// 	return (pipe_fd[0]);
// }

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

void	handle_input_redir(t_cmd *cmd, t_exec_cmd *vars, int *g_exit_status)
{
	if (vars->i == 0 || cmd->input_file)
	{
		if (setup_input(cmd, vars, g_exit_status) < 0)
			exit(*g_exit_status);
	}
	else if (vars->i > 0)
	{
		if (dup2(vars->pipefd[vars->i - 1][0], STDIN_FILENO) < 0)
		{
			perror("dup2 input");
			exit(EXIT_FAILURE);
		}
	}
}

void	handle_output_redir(t_cmd *cmd, int cmd_count, t_exec_cmd *vars)
{
	if (vars->i == cmd_count - 1 || cmd->output_file)
	{
		if (cmd->output_file)
		{
			if (handle_last_command_output(cmd, vars) < 0)
				exit(EXIT_FAILURE);
		}
		else
			vars->fdout = dup(vars->ori_out);
	}
	else if (vars->i < cmd_count - 1)
	{
		if (dup2(vars->pipefd[vars->i][1], STDOUT_FILENO) < 0)
		{
			perror("dup2 output");
			exit(EXIT_FAILURE);
		}
	}
}
