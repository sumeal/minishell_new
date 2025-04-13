/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muzz <muzz@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 11:51:58 by abin-moh          #+#    #+#             */
/*   Updated: 2025/04/08 11:23:39 by muzz             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_command(t_cmd *cmd)
{
	t_cmd	*temp;
	int		count;

	count = 0;
	temp = cmd;
	while (temp)
	{
		count++;
		temp = temp->next;
	}
	return (count);
}

void	setup_io(t_cmd *cmd, int i, int cmd_count, t_exec_cmd *vars, int *g_exit_status)
{
	if (i == 0)
	{
		setup_input(cmd, vars, g_exit_status);
	}
	else if (i > 0)
	{
		if (dup2(vars->pipefd[i - 1][0], STDIN_FILENO) < 0)
		{
			perror("dup2 input");
			exit(EXIT_FAILURE);
		}
	}
	if (i == cmd_count - 1)
	{
		if (cmd->output_file)
		{
			if (handle_last_command_output(cmd, vars) < 0)
				exit(EXIT_FAILURE);
		}
		else
			vars->fdout = dup(vars->ori_out);
	}
	else if (i < cmd_count - 1)
	{
		if (dup2(vars->pipefd[i][1], STDOUT_FILENO) < 0)
		{
			perror("dup2 output");
			exit(EXIT_FAILURE);
		}
	}
	dup2(vars->fdin, STDIN_FILENO);
	close(vars->fdin);
	dup2(vars->fdout, STDOUT_FILENO);
	close(vars->fdout);
	i = 0;
	if (cmd_count != 1)
	{
		while (i < cmd_count)
		{
			close(vars->pipefd[i][0]);
			close(vars->pipefd[i][1]);
			i++;
		}
	}
}

void	run_the_commands(t_cmd *cmd, int cmd_count, int *g_exit_status, t_exec_cmd *vars, char ***envp)
{
	int		i;
	pid_t	pid[cmd_count];
	t_cmd	*cur;
	char	*path;
	
	cur = cmd;
	save_original_fd(vars);
	i = 0;
	while (i < cmd_count)
	{
		pid[i] = fork();
		setup_signal_child();
		if (pid[i] < 0)
		{
			perror("fork");
			return ;
		}
		if (pid[i] == 0)
		{
			setup_io(cur, i, cmd_count, vars, g_exit_status);
			if (ft_strncmp(cur->argv[0], "./", 2) == 0)
				execve(cur->argv[0],cur->argv, *envp);
			path = get_path(cur->argv[0], *envp);
			if (path == NULL)
			{
				printf("%s: command not found\n", cur->argv[0]);
				exit(127);
			}
			execve(path, cur->argv, *envp);
			perror("execve");
			exit(EXIT_FAILURE);
		}
		if (cur->next)
			cur = cur->next;
		i++;
	}
	i = 0;
	if (cmd_count != 1)
	{
		while (i < cmd_count)
		{
			close(vars->pipefd[i][0]);
			close(vars->pipefd[i][1]);
			i++;
		}
	}
	i = 0;
	while (i < cmd_count)
	{
		waitpid(pid[i],&vars->status, 0);
		i++;
	}
	set_exit_status(vars, g_exit_status);
	restore_original_fd(vars);
}

void	execute_multiple_commands(t_cmd **cmd,
	char ***envp, int *g_exit_status)
{
	int			count;
	int			i;
	t_exec_cmd	vars;

	count = count_command(*cmd);
	vars.pipefd = malloc(sizeof(int *) * count);
	i = 0;
	while (i < count)
	{
		vars.pipefd[i] = malloc(sizeof(int) * 2);
		if (pipe(vars.pipefd[i]) == -1)
		{
			perror("pipe");
			return ;
		}
		i++;
	}
	run_the_commands(*cmd, count, g_exit_status, &vars, envp);
}

void	execution(t_cmd *commands, char ***mini_envp, int *g_exit_status)
{
	int	fok;
	t_exec_cmd	vars;

	if (commands->next ==NULL)
	{
		fok = execute_builtin_command(&commands, mini_envp, g_exit_status);
		if (fok == 0)
		{
			run_the_commands(commands, 1, g_exit_status, &vars, mini_envp);
		}
	}
	else
		execute_multiple_commands(&commands, mini_envp, g_exit_status);
}

// void	execute_commands(t_cmd *cmd_list, char **envp, int *g_exit_status)
// {
// 	t_exec_cmd	vars;

// 	save_original_fd(&vars);
// 	if (setup_input(cmd_list, &vars, g_exit_status) < 0)
// 		return ;
// 	while (cmd_list)
// 	{
// 		dup2(vars.fdin, STDIN_FILENO);
// 		close(vars.fdin);
// 		if (setup_output(cmd_list, &vars) < 0)
// 			return ;
// 		dup2(vars.fdout, STDOUT_FILENO);
// 		close(vars.fdout);
// 		vars.builtin_executed
// 			= execute_builtin_command(&cmd_list, &envp, g_exit_status);
// 		if (vars.builtin_executed == 0)
// 		{
// 			if (execute_external_command(cmd_list, &vars, envp, g_exit_status)
// 				< 0)
// 				break ;
// 		}
// 		cmd_list = cmd_list->next;
// 	}
// 	restore_original_fd(&vars);
// }

int	execute_command(t_cmd *cmd,
	t_exec_cmd *vars, char **envp, int *g_exit_status)
{
	char	*path;

	(void)g_exit_status;
	setup_signal_child();
	vars->pid = fork();
	if (vars->pid == 0)
	{
		path = get_path(cmd->argv[0], envp);
		if (path == NULL)
		{
			printf("%s: command not found\n", cmd->argv[0]);
			exit(127);
		}
		execve(path, cmd->argv, envp);
		perror("execve");
		exit(EXIT_FAILURE);
	}
	else if (vars->pid < 0)
		return (print_error("fork", -1));
	return (0);
}

int	execute_external_command(t_cmd *cmd_list,
	t_exec_cmd *vars, char **envp, int *g_exit_status)
{
	if (execute_command(cmd_list, vars, envp, g_exit_status) < 0)
		return (-1);
	if (cmd_list->next == NULL)
	{
		waitpid(vars->pid, &vars->status, 0);
		if (vars->status)
			set_exit_status(vars, g_exit_status);
	}
	return (0);
}

int	execute_builtin_command(t_cmd **cmd,
	char ***envp, int *g_exit_status)
{
	if (ft_strcmp((*cmd)->argv[0], "echo") == 0
		&& (*cmd)->argv[1]
		&& ft_strcmp((*cmd)->argv[1], "$?") == 0)
		return (print_exit_status(g_exit_status));
	else if (ft_strcmp((*cmd)->argv[0], "cd") == 0 && (*cmd)->prev == NULL
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
