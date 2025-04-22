/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abin-moh <abin-moh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 11:51:58 by abin-moh          #+#    #+#             */
/*   Updated: 2025/04/22 21:22:47 by abin-moh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	create_child_processes(t_cmd *cmd, t_exec_cmd *vars,
			int *g_exit_status, char ***envp)
{
	t_cmd	*cur;

	vars->i = 0;
	cur = cmd;
	while (vars->i < (*vars).cmd_count)
	{
		(*vars).pid[vars->i] = fork();
		if (vars->pid[vars->i] < 0)
		{
			perror("fork");
			return ;
		}
		if (vars->pid[vars->i] == 0)
		{
			execute_command_in_child(cur, vars, g_exit_status, envp);
		}
		else
		{
			signal(SIGINT, SIG_IGN);
			signal(SIGQUIT, SIG_IGN);
			signal(SIGINT, handle_signal_child);
			signal(SIGQUIT, handle_signal_child_exit);
		}
		if (cur->next)
			cur = cur->next;
		vars->i++;
	}
}

void	process_heredoc(t_cmd *cmd, int input_fd, t_exec_cmd *vars, int *g_exit_status)
{
	char	*line;

	while (1)
    {
        line = readline("> ");
        if (!line) // End of input (Ctrl+D)
        {
	         exit(EXIT_SUCCESS);
        }
        if (ft_strcmp(line, cmd->hd_delimeter) == 0)
        {
            free(line);
        	exit(EXIT_SUCCESS);
        }
        line = expand_lexem(line, vars->envp, *g_exit_status);
        ft_putendl_fd(line, input_fd);
        free(line);
    }
    exit(EXIT_FAILURE); // Exit with failure if the loop ends unexpectedly
}

void	handle_heredoc(t_cmd *cmd, t_exec_cmd *vars, int *g_exit_status)
{
	pid_t	hd_pid;
	int		status;
	int		fd;
	
	fd = open("/tmp/temporary", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	hd_pid = fork();
	if (hd_pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		process_heredoc(cmd, fd, vars, g_exit_status);
		close(fd);
		exit(EXIT_SUCCESS);
	}
	else
	{
		signal(SIGINT, handle_signal_heredoc);
		waitpid(hd_pid, &status, 0);
		if (WIFSIGNALED(status))
		{
			*g_exit_status = 130;
			cmd->is_hd = 99;
		}
	}
	close(fd);
}

void	here_doc_process(t_cmd *cmd, t_exec_cmd *vars, int *g_exit_status)
{
	if (cmd->is_hd)
		handle_heredoc(cmd, vars, g_exit_status);
	else
		return ;
}

void	run_the_commands(t_cmd *cmd, int *g_exit_status,
			t_exec_cmd *vars, char ***envp)
{
	vars->envp = *envp;
	vars->pid = malloc(sizeof(pid_t) * vars->cmd_count);
	if (!vars->pid)
	{
		perror("pid");
		exit(1);
	}
	save_original_fd(vars);
	here_doc_process(cmd, vars, g_exit_status);
	if (cmd->is_hd == 99)
		return ;
	create_child_processes(cmd, vars, g_exit_status, envp);
	if (vars->cmd_count != 1)
		closing_pipes(&vars);
	wait_for_child(vars);
	change_signal(0);
	set_exit_status(vars, g_exit_status);
	restore_original_fd(vars);
	free(vars->pid);
	if (vars->cmd_count != 1)
		free_pipe(vars);
}

void	execute_multiple_commands(t_cmd **cmd,
	char ***envp, int *g_exit_status)
{
	t_exec_cmd	vars;

	vars.cmd_count = count_command(*cmd);
	if (vars.cmd_count > 1)
	{
		vars.pipefd = malloc(sizeof(int *) * (vars.cmd_count - 1));
		if (!vars.pipefd)
		{
			perror("malloc for pipefd");
			return ;
		}
		vars.i = 0;
		while (vars.i < vars.cmd_count - 1)
		{
			vars.pipefd[vars.i] = malloc(sizeof(int) * 2);
			if (pipe(vars.pipefd[vars.i]) == -1)
			{
				perror("pipe");
				return ;
			}
			vars.i++;
		}
	}
	run_the_commands(*cmd, g_exit_status, &vars, envp);
}

void	execution(t_cmd *commands, char ***mini_envp, int *g_exit_status)
{
	int			fok;
	t_exec_cmd	vars;

	if (commands->next == NULL && !commands->append
		&& !commands->input_file && !commands->output_file)
	{
		fok = execute_builtin_command(&commands, mini_envp, g_exit_status);
		if (fok == 0)
		{
			vars.cmd_count = 1;
			run_the_commands(commands, g_exit_status, &vars, mini_envp);
		}
	}
	else
		execute_multiple_commands(&commands, mini_envp, g_exit_status);
}

int	execute_builtin_command(t_cmd **cmd,
	char ***envp, int *g_exit_status)
{
	if (ft_strcmp((*cmd)->argv[0], "cd") == 0 && (*cmd)->prev == NULL
		&& (*cmd)->next == NULL)
	{
		change_directory(cmd, envp, g_exit_status);
		return (1);
	}
	else if (ft_strcmp((*cmd)->argv[0], "echo") == 0)
		return (print_echo((*cmd)->argv, g_exit_status));
	else if (ft_strcmp((*cmd)->argv[0], "env") == 0)
		return (print_env(cmd, *envp, g_exit_status));
	else if (ft_strcmp((*cmd)->argv[0], "pwd") == 0)
		return (print_pwd(*envp, g_exit_status));
	else if (ft_strcmp((*cmd)->argv[0], "export") == 0)
		return (export_variable((*cmd)->argv, envp, g_exit_status));
	else if (ft_strcmp((*cmd)->argv[0], "unset") == 0)
		return (unset_env((*cmd), *envp, g_exit_status));
	else if (ft_strcmp((*cmd)->argv[0], "exit") == 0)
		return (exit_function((*cmd), *envp, g_exit_status));
	return (0);
}
